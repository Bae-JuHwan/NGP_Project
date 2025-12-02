#include "obstacle_network.h"
#include <stdio.h>

obstacle_Bong g_bongObstacle; // 전역 변수 정의
extern CRITICAL_SECTION g_cs_client;
int reciveBongCount = 0;
bool recv_BongObstacle(SOCKET sock)
{
    if (sock == INVALID_SOCKET) {
        printf("[경고] 소켓이 유효하지 않습니다\n");
        return false;
    }

    obstacle_Bong_Net netData;

    // 서버로부터 장애물 정보 패킷 수신
    if (!recv_all(sock, (char*)&netData, sizeof(obstacle_Bong_Net))) {
        int err = WSAGetLastError();
        if (err != WSAEWOULDBLOCK) {
            printf("[에러] recv_BongObstacle() 실패 - 에러코드: %d\n", err);
        }
        return false;
    }

    obstacle_Bong obs_info;

    obs_info.pos1 = glm::vec3(netData.pos1[0], netData.pos1[1], netData.pos1[2]);
    obs_info.dir1 = glm::vec3(netData.dir1[0], netData.dir1[1], netData.dir1[2]);

    obs_info.pos2 = glm::vec3(netData.pos2[0], netData.pos2[1], netData.pos2[2]);
    obs_info.dir2 = glm::vec3(netData.dir2[0], netData.dir2[1], netData.dir2[2]);

    g_bongObstacle = obs_info;

    // 로그 출력
    reciveBongCount++;
    if (reciveBongCount % 100 == 0) {
        printf("장애물 정보 받음\n");
        printf("pos1: (%.2f, %.2f, %.2f)\n", obs_info.pos1.x, obs_info.pos1.y, obs_info.pos1.z);
        printf("dir1: (%.2f, %.2f, %.2f)\n", obs_info.dir1.x, obs_info.dir1.y, obs_info.dir1.z);
        printf("pos2: (%.2f, %.2f, %.2f)\n", obs_info.pos2.x, obs_info.pos2.y, obs_info.pos2.z);
        printf("dir2: (%.2f, %.2f, %.2f)\n", obs_info.dir2.x, obs_info.dir2.y, obs_info.dir2.z);
    }

    return true;
}

bool recv_all(SOCKET sock, char* buf, int size)
{
    int received = 0;
    while (received < size) {
        int ret = recv(sock, buf + received, size - received, 0);
        if (ret <= 0) return false;  // 실패 or 종료
        received += ret;
    }
    return true;
}