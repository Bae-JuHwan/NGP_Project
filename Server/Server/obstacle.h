#pragma once

#include <WinSock2.h>
#include "stdafx.h"
#include "Common.h"
#include "stdafx.h"

// 서버 -> 클라 첫번째 장애물(봉) 보내기 구조체
#pragma pack(1)
struct Bong_Obstacle {
    glm::vec3 pos1;
    glm::vec3 dir1;
    glm::vec3 pos2;
    glm::vec3 dir2;
};
#pragma pack()

#pragma pack(1)
struct Door_Obstacle {
    glm::vec3 pos1;
    glm::vec3 dir1;
    glm::vec3 pos2;
    glm::vec3 dir2;
};
#pragma pack()


extern Bong_Obstacle g_bongObstacle;
extern Door_Obstacle g_doorObstacle;

bool S2C_BongObstacle(SOCKET sock, const Bong_Obstacle& obs_info);
bool S2C_DoorObstacle(SOCKET sock, const Door_Obstacle& obs_info);
void UpdateBongObstacle();
void UpdateDoorObstacle();
//bool Broadcast_BongObstacle(const Moving_Obstacle& obs_info , ClientInfo g_clients[MAX_CLIENTS]);
void InitBongObstacle();
void InitDoorObstacle();