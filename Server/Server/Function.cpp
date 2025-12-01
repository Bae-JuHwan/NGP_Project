#include "Function.h"


bool IsAllPlayersReady()    //3명 모두 접속했니?       
{
    bool ready = false;

    EnterCriticalSection(&g_cs);
    if (g_clientCount >= 3)   // 원하는 최소 인원
        ready = true;
    LeaveCriticalSection(&g_cs);

    return ready;
}

bool S2C_isPlayerReady(SOCKET sock, int num) {  // 카운트다운 전송함수
    int data = num;

    // 엔디안 변환
    int send_data = htonl(data);

    int retval = send(sock, (char*)&send_data, sizeof(send_data), 0);
    if (retval == SOCKET_ERROR) {
        err_display("send()");
        return false;
    }

    return true;
}

void SendToAllClients(int data)
{
    int send_data = htonl(data); // 엔디안 변환
    EnterCriticalSection(&g_cs);

    for (int i = 0; i < g_clientCount; ++i) {
        if (g_clients[i].isActive) {
            int retval = send(g_clients[i].sock, (char*)&send_data, sizeof(send_data), 0);
            if (retval == SOCKET_ERROR) {
                err_display("send()");
            }
        }
    }

    LeaveCriticalSection(&g_cs);
}

DWORD WINAPI CountdownThread(LPVOID arg) // 쓰레드 함수
{
    // 3초마다 3 → 2 → 1 보내기
    SendToAllClients(3);       // 모두에게 3 보내기
    Sleep(3000);               // 1초 대기
    SendToAllClients(2);       // 모두에게 2 보내기
    Sleep(3000);               // 1초 대기
    SendToAllClients(1);       // 모두에게 1 보내기
    Sleep(3000);               // 1초 대기

    // 끝나면 -1 보내서 클라이언트에서 쓰레드 종료
    SendToAllClients(-1);

    return 0;
}