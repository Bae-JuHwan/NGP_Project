#include "Function.h"
#pragma comment(lib, "ws2_32.lib")

#define SERVERPORT 9000

/*  전달사항아닌 전달사항


g_clientCount 이걸로만 판단하면 안될 것 같음. 들어왔다가 나갔을때 저 수가 줄어들게 해야할듯.
그리고 재접 가능하게 할 것인지, 다 접속했을때 세명 다 레디 눌러야 시작하게 할 것인지>?
접속하자마자 게임 시작은 약간 아쉬운 부분이 있을수도있을것같음. 그냥 의견임.

*/
#pragma pack(1)
struct GamePacket_S2C {
    character otherPlayers[2];    // 다른 플레이어 2명
    Moving_Obstacle bongObstacle;   // 장애물 정보
    Moving_Obstacle doorObstacle;   // 장애물 정보
    Rotating_Obstacle jumpbarObstacle;
    Rotating_Obstacle vFanObstacle;
    Rotating_Obstacle hFanObstacle;
};
#pragma pack()

CRITICAL_SECTION g_cs;  // 임계영역
int g_clientCount = 0;

// 캐릭터 정보 저장하기 위해서 클라이언트 정보 구조체 배열
ClientInfo g_clients[MAX_CLIENTS];


// 서버에서 클라이언트로 캐릭터 정보 전송
int characterSendCount = 0;
bool countEnd = false;  // 카운트다운 종료 후 게임 시작하는 플래그
bool g_countdown = true;   //카운트다운 함수 한번만 실행하게 하는 용


// 충돌 처리 함수 (아직 미구현)
bool CheckCollision(const character& ch) {
    return false; // 임시 반환
}
bool S2C_GameState(SOCKET sock, int clientId) {
    GamePacket_S2C packet;

    // 해당 클라이언트를 제외한 다른 플레이어들 정보 채우기
    int idx = 0;
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (i != clientId - 1 && g_clients[i].isActive) {
            packet.otherPlayers[idx++] = g_clients[i].charInfo;
        }
    }

    // 장애물 정보
    packet.bongObstacle = g_bongObstacle;
    packet.doorObstacle = g_doorObstacle;
    packet.jumpbarObstacle = g_jumpbarObstacle;
    packet.vFanObstacle = g_vFanObstacle;
    packet.hFanObstacle = g_hFanObstacle;

    printf("봉 장애물 위치1 : x=%f, y=%f, z=%f\n", g_bongObstacle.pos1.x, g_bongObstacle.pos1.y, g_bongObstacle.pos1.z);
    printf("봉 장애물 위치2 : x=%f, y=%f, z=%f\n", g_bongObstacle.pos2.x, g_bongObstacle.pos2.y, g_bongObstacle.pos2.z);

    printf("문 장애물 위치1 : x=%f, y=%f, z=%f\n", g_doorObstacle.pos1.x, g_doorObstacle.pos1.y, g_doorObstacle.pos1.z);
    printf("문 장애물 위치2 : x=%f, y=%f, z=%f\n", g_doorObstacle.pos2.x, g_doorObstacle.pos2.y, g_doorObstacle.pos2.z);

    // 한 번에 전송
    if (!send(sock, (char*)&packet, sizeof(GamePacket_S2C), 0)) {
        return false;
    }

    return true;
}

// 클라이언트로부터 캐릭터 정보 받기
bool recv_character(SOCKET sock, character& ch) {
    int totalReceived = 0;
    int remaining = sizeof(character);
    char* buffer = (char*)&ch;

    while (remaining > 0) {
        int retval = recv(sock, buffer + totalReceived, remaining, 0);
        if (retval == SOCKET_ERROR) {
            err_display("recv() - recv_character");
            return false;
        }
        if (retval == 0) {
            printf("클라이언트 연결 종료\n");
            return false;
        }
        totalReceived += retval;
        remaining -= retval;
    }
    return true;
}



// 클라이언트 스레드 함수
DWORD WINAPI ClientThread(LPVOID arg) {
    SOCKET client_sock = *(SOCKET*)arg;
    free(arg);
    int client_id;

    EnterCriticalSection(&g_cs);
    client_id = ++g_clientCount;
    g_clients[client_id - 1].sock = client_sock;
    g_clients[client_id - 1].id = client_id;
    g_clients[client_id - 1].isActive = true;
    LeaveCriticalSection(&g_cs);

    printf("클라이언트 %d번 접속 완료\n", client_id);
    S2C_ClientOrder(client_sock, client_id);   //몇번째 클라인지 보내주기


    // TODO
    int receive_count = 0;
    int send_count[MAX_CLIENTS]{};
    while(g_clientCount < 3) {
		//printf("클라이언트 %d번 대기중... 현재 접속자 수: %d\n", client_id, g_clientCount);
        Sleep(100); // 3명 접속 대기
	}
    
    //아무 쓰레드 중 하나가 확인하고 카운트다운 시작하면. 전체로 보내줌. 그 후 닫아서 다른 쓰레드는 못보게 함
    if (IsAllPlayersReady()&& g_countdown) {
        EnterCriticalSection(&g_cs);
        CountdownThread(nullptr);
        g_countdown = false;
        LeaveCriticalSection(&g_cs);
    }


    while (countEnd) {
        character received_char;

        // 클라이언트로부터 캐릭터 정보 수신
        if (!recv_character(client_sock, received_char)) {
            break;  // 수신 실패 시 루프 종료
        }

        receive_count++;

        // 수신한 데이터 출력
        if (receive_count % 100 == 0) {
            printf("\n=== [클라이언트 %d] 수신 %d회 ===\n", g_clients[client_id - 1].id, receive_count);
            printf("  Position: (%.2f, %.2f, %.2f)\n",
                received_char.position.x, received_char.position.y, received_char.position.z);
            printf("  Direction: (%.2f, %.2f, %.2f)\n",
                received_char.direction.x, received_char.direction.y, received_char.direction.z);
            printf("  ArmLegSwingAngle: %.2f\n", received_char.ArmLegSwingAngle);
            printf("  isCollision: %s\n", received_char.isCollision ? "true" : "false");
            printf("\n");
        }

        // 임계영역 진입 - 데이터 저장
        EnterCriticalSection(&g_cs);
        g_clients[client_id - 1].charInfo = received_char;
        UpdateMovingObstacle();
        UpdateRotatingObstacle();

        // 게임 상태 전송
        S2C_GameState(client_sock, client_id);
        LeaveCriticalSection(&g_cs);
    }

    closesocket(client_sock);
    printf("클라이언트 %d번 연결 종료\n", client_id);
    EnterCriticalSection(&g_cs);
    g_clientCount--;
    LeaveCriticalSection(&g_cs);
    return 0;
}

int main() {
    WSADATA wsa;
    SOCKET listen_sock, client_sock;
    struct sockaddr_in serveraddr, clientaddr;
    int addrlen = sizeof(clientaddr);

    // Winsock 초기화
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("WSAStartup 실패\n");
        return 1;
    }

    InitializeCriticalSection(&g_cs);
    InitMovingObstacle();
    InitRotatingObstacle();

    listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET) {
        printf("소켓 생성 실패\n");
        return 1;
    }

    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);

    if (bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == SOCKET_ERROR) {
        printf("bind 실패: %d\n", WSAGetLastError());
        closesocket(listen_sock);
        return 1;
    }

    if (listen(listen_sock, SOMAXCONN) == SOCKET_ERROR) {
        printf("listen 실패\n");
        closesocket(listen_sock);
        return 1;
    }

    // 클라 배열 초기화
    for (int i = 0; i < MAX_CLIENTS; i++) {
        g_clients[i].sock = INVALID_SOCKET;
        g_clients[i].isActive = false;
    }


    printf("서버가 포트 %d에서 대기 중...\n", SERVERPORT);

   
    // 클라이언트 접속 루프 (메인 스레드에서 블록 가능)
    int clientCount = 0;
    while (clientCount < MAX_CLIENTS) {
        client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET) continue;

        EnterCriticalSection(&g_cs);
        int id = clientCount++;
        g_clients[id].sock = client_sock;
        g_clients[id].id = id + 1;
        g_clients[id].isActive = true;
        LeaveCriticalSection(&g_cs);

        SOCKET* pSock = (SOCKET*)malloc(sizeof(SOCKET));
        *pSock = client_sock;
        HANDLE hThread = CreateThread(NULL, 0, ClientThread, (LPVOID)pSock, 0, NULL);
        if (hThread) CloseHandle(hThread);

        printf("클라이언트 %d 접속 완료\n", id + 1);
    }


    // 서버 종료 처리 (무한 루프 대신 플래그 사용 가능)
    while (true) Sleep(1000);

    DeleteCriticalSection(&g_cs);
    closesocket(listen_sock);
    WSACleanup();
    return 0;
}