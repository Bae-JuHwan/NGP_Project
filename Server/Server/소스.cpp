#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

#define SERVERPORT 9000
#define MAX_CLIENTS 3

CRITICAL_SECTION g_cs;  // 임계영역
int g_clientCount = 0;

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