#pragma once
#include <WinSock2.h>
#include"stdafx.h"

#pragma pack(1)
struct Moving_Obstacle {
    int type;
    glm::vec3 pos1;
    glm::vec3 dir1;
    glm::vec3 pos2;
    glm::vec3 dir2;
};
#pragma pack()

extern Moving_Obstacle g_bongObstacle;
extern Moving_Obstacle g_doorObstacle;

bool recv_MovingObstacle(SOCKET sock);