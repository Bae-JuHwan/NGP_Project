#include"Common.h"
#include <gtc/matrix_transform.hpp>
#include <gl/glew.h>
#pragma comment(lib, "ws2_32.lib")

#define SERVERPORT 9000
#define MAX_CLIENTS 3

CRITICAL_SECTION g_cs;  // 임계영역
int g_clientCount = 0;

// 캐릭터 구조체
#pragma pack(1)
struct character {
    glm::vec3 position;
    glm::vec3 direction;
    GLfloat ArmLegSwingAngle;
    bool isCollision;
};
#pragma pack()
// 클라이언트 정보 구조체 추가
struct ClientInfo {
    SOCKET sock;
    int id;
    character charInfo;
    bool isActive;
};

// 충돌 처리 함수 (아직 미구현)
bool CheckCollision(const character& ch) {
    return false; // 임시 반환
}

// 클라이언트로부터 캐릭터 정보 받기
bool recv_character(SOCKET sock, character& ch) {
    int retval = recv(sock, (char*)&ch, sizeof(character), 0);
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

    // 전송 성공
    printf("[서버] 캐릭터 정보 전송 완료 (%d 바이트)\n", retval);
    return true;
}
// 클라이언트 스레드 함수
DWORD WINAPI ClientThread(LPVOID arg) {
    SOCKET client_sock = *(SOCKET*)arg;
    free(arg);
    int client_id;

    EnterCriticalSection(&g_cs);
    client_id = ++g_clientCount;
    LeaveCriticalSection(&g_cs);

    printf("클라이언트 %d번 접속 완료\n", client_id);

    // TODO
    int receive_count = 0;
    while (true) {
        character received_char;

        // 클라이언트로부터 캐릭터 정보 수신
        if (!recv_character(client_sock, received_char)) {
            break;  // 수신 실패 시 루프 종료
        }

        receive_count++;

        // 수신한 데이터 출력
        if (receive_count % 100 == 0) {
            printf("\n=== [클라이언트 %d] 수신 %d회 ===\n", client_id, receive_count);
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
        // 여기에 클라이언트 정보 저장 (나중에 구현)
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