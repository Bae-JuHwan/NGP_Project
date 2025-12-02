#pragma once

#include <WinSock2.h>
#include "stdafx.h"
#include "Common.h"
#include "stdafx.h"

struct obstacle_Bong {
    glm::vec3 pos1;
    glm::vec3 dir1;
    glm::vec3 pos2;
    glm::vec3 dir2;
};

#pragma pack(push, 1)
struct obstacle_Bong_Net {
    float pos1[3];
    float dir1[3];
    float pos2[3];
    float dir2[3];
};
#pragma pack(pop)

extern obstacle_Bong g_bongObstacle;

bool S2C_BongObstacle(SOCKET sock, const obstacle_Bong& obs_info);
void UpdateBongObstacle();
bool Broadcast_BongObstacle(const obstacle_Bong& obs_info , ClientInfo g_clients[MAX_CLIENTS]);
void InitBongObstacle();