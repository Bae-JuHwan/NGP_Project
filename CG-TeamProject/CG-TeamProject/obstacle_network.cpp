#include "obstacle_network.h"
#include <stdio.h>

obstacle_Bong g_bongObstacle; // 전역 변수 정의
extern CRITICAL_SECTION g_cs_client;

bool recv_BongObstacle(SOCKET sock) {
    char* buf = (char*)&g_bongObstacle;
    int remaining = sizeof(obstacle_Bong);
    int total_received = 0;

    while (remaining > 0) {
        int retval = recv(sock, buf + total_received, remaining, 0);

        if (retval == SOCKET_ERROR) {
            // 여기에 에러 처리 (err_display 등)
            return false;
        }
        if (retval == 0) {
            // 연결 종료
            return false;
        }

        total_received += retval;
        remaining -= retval;
        printf("\n[수신] 장애물 정보 수신 완료 (%d 바이트)\n", retval);
    }

    //EnterCriticalSection(&g_cs_client);
    //LeaveCriticalSection(&g_cs_client);

    printf("[클라] 봉 장애물 정보 수신 완료 (%d 바이트)\n", total_received);
    return true;
}