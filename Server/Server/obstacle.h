#pragma once

#include <WinSock2.h>
#include "stdafx.h"
#include "Common.h"
#include "stdafx.h"

// 서버 -> 클라 첫번째 장애물(봉) 보내기 구조체
#pragma pack(1)
struct obstacle_Bong {
    glm::vec3 pos1;
    glm::vec3 dir1;
    glm::vec3 pos2;
    glm::vec3 dir2;
};
#pragma pack()

extern obstacle_Bong g_bongObstacle;

bool S2C_BongObstacle(SOCKET sock, const obstacle_Bong& obs_info);
void UpdateBongObstacle();
bool Broadcast_BongObstacle(const obstacle_Bong& obs_info , ClientInfo g_clients[MAX_CLIENTS]);
void InitBongObstacle();