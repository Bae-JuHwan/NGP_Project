#include "Common.h"
#include <glm/gtc/matrix_transform.hpp>
#include <gl/glew.h>
#include "obstacle.h"

#pragma comment(lib, "ws2_32.lib")

#define SERVERPORT 9000

/*  전달사항아닌 전달사항


g_clientCount 이걸로만 판단하면 안될 것 같음. 들어왔다가 나갔을때 저 수가 줄어들게 해야할듯.
그리고 재접 가능하게 할 것인지, 다 접속했을때 세명 다 레디 눌러야 시작하게 할 것인지>?
접속하자마자 게임 시작은 약간 아쉬운 부분이 있을수도있을것같음. 그냥 의견임.

*/

CRITICAL_SECTION g_cs;  // 임계영역
int g_clientCount = 0;


// 캐릭터 정보 저장하기 위해서 클라이언트 정보 구조체 배열
ClientInfo g_clients[MAX_CLIENTS];
// 충돌 처리 함수 (아직 미구현)
bool CheckCollision(const character& ch) {
    return false; // 임시 반환
}

// 클라이언트로부터 캐릭터 정보 받기
bool recv_character(SOCKET sock, character& ch) {
    int retval = recv(sock, (char*)&ch, sizeof(character), 0);
    printf("전송받은  데이터 크기 %d)\n", retval);
    if (retval == SOCKET_ERROR) {
        err_display("recv() - recv_character");
        return false;
    }
    if (retval == 0) {
        printf("클라이언트 연결 종료\n");
        return false;
    }
    return true;
}

// 서버에서 클라이언트로 캐릭터 정보 전송
bool S2C_Character(SOCKET sock, const character& char_info) {
    // 소켓이 유효한지 확인
    if (sock == INVALID_SOCKET) {
        printf("[경고] 소켓이 유효하지 않습니다\n");
        return false;
    }

    // 클라이언트에게 캐릭터 정보 전송
    int retval = send(sock, (char*)&char_info, sizeof(character), 0);

    if (retval == SOCKET_ERROR) {
        int err = WSAGetLastError();
        printf("[에러] S2C_Character() 전송 실패 - 에러코드: %d\n", err);
        return false;
    }

    if (retval != sizeof(character)) {
        printf("[경고] 전송된 데이터 크기 불일치 (예상: %zu, 실제: %d)\n", sizeof(character), retval);
    }

    return true;
}

int S2C_ClientOrder(SOCKET sock, int order) {   //클라에게 몇번째 클라인지 보내주는 함수
    int retval;
    int data = htonl(order); // 엔디안 변환(필수)

    retval = send(sock, (char*)&data, sizeof(data), 0);
    printf("전송된 데이터 크기 %d)\n", retval);
    if (retval == SOCKET_ERROR)
    {
        err_display("send()");
        return -1;
    }

    return retval;
}

bool IsAllPlayersReady()    //3명 모두 접속했니?       
{
    bool ready = false;

    EnterCriticalSection(&g_cs);
    if (g_clientCount >= 3)   // 원하는 최소 인원
        ready = true;
    LeaveCriticalSection(&g_cs);

    return ready;
}

bool S2C_isPlayerReady(SOCKET sock) {  // 플레이어 모두 접속 완료 됐는지 전송 함수 
    bool ready = IsAllPlayersReady();
    int data = ready ? 1 : 0;

    // 엔디안 변환
    int send_data = htonl(data);

    int retval = send(sock, (char*)&send_data, sizeof(send_data), 0);
    if (retval == SOCKET_ERROR) {
        err_display("send()");
        return false;
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


	//S2C_ClientOrder(client_sock, client_id);   //몇번째 클라인지 보내주기
    //이 부분 수정 필요할 것 같음------ while 돌릴예정.

    //S2C_isPlayerReady(client_sock);//3명 접속했는지 확인하고 맞으면 클라에게 보내기
    //----------

    // TODO
    int receive_count = 0;
    int send_count[MAX_CLIENTS];
    while (true) {
        
        printf("장애물 전송 로직 \n");

        EnterCriticalSection(&g_cs);
        UpdateBongObstacle();
        Broadcast_BongObstacle(g_bongObstacle , g_clients);
        LeaveCriticalSection(&g_cs);
        printf("장애물 전송 로직 끝 \n");
       

        //Sleep(16); // 60 FPS 주기로 실행
        character received_char;


        
        // 클라이언트로부터 캐릭터 정보 수신
        if (!recv_character(client_sock, received_char)) {
            std::cout << "캐릭터 정보 수신 실패" << '\n';
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
 
        EnterCriticalSection(&g_cs);
        g_clients[client_id - 1].charInfo = received_char; //캐릭터 정보 저장
        // 다른 클라이언트들에게 캐릭터 정보 전송
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (i != client_id - 1 && g_clients[i].isActive) { // 자기 자신 제외
                if (!S2C_Character(g_clients[i].sock, received_char)) {
                    printf("클라이언트 %d번에게 캐릭터 정보 전송 실패\n", g_clients[i].id);
                }
                else {
                    if (send_count[i] % 100 == 0) {
                        printf("[서버] 클라이언트 %d 캐릭터 정보 전송 완료 송신 %d회 \n", g_clients[client_id - 1].id, send_count);
                        send_count[i]++;
                    }
                }
            }
        }
        LeaveCriticalSection(&g_cs);
    }

    closesocket(client_sock);
    printf("클라이언트 %d번 연결 종료\n", client_id);
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

    printf("서버가 포트 %d에서 대기 중...\n", SERVERPORT);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        g_client_list[i] = INVALID_SOCKET;
    }

    while (1) {
        client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET) {
            printf("accept 실패\n");
            continue;
        }

        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(clientaddr.sin_addr), clientIP, INET_ADDRSTRLEN);
        printf("클라이언트 접속: IP=%s, PORT=%d\n", clientIP, ntohs(clientaddr.sin_port));

        // 스레드 인자로 전달할 소켓 복사본 동적 할당
        SOCKET* pClientSock = (SOCKET*)malloc(sizeof(SOCKET));
        *pClientSock = client_sock;

        HANDLE hThread = CreateThread(NULL, 0, ClientThread, (LPVOID)pClientSock, 0, NULL);
        if (hThread == NULL) {
            printf("스레드 생성 실패\n");
            closesocket(client_sock);
            free(pClientSock);
        }
        else {
            CloseHandle(hThread);
        }

        if (g_clientCount >= MAX_CLIENTS) {
            printf("최대 접속자 수 도달\n");
            break;
        }
    }

    DeleteCriticalSection(&g_cs);
    closesocket(listen_sock);
    WSACleanup();
    return 0;
}