#pragma once
#include <WinSock2.h>
#include"stdafx.h"

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

bool recv_BongObstacle(SOCKET sock);
bool recv_DoorObstacle(SOCKET sock);