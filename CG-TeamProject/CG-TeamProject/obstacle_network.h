#pragma once
#include <WinSock2.h>
#include"stdafx.h"

#pragma pack(1)
struct obstacle_Bong {
    glm::vec3 pos1;
    glm::vec3 dir1;
    glm::vec3 pos2;
    glm::vec3 dir2;
};
#pragma pack()

extern obstacle_Bong g_bongObstacle;

bool recv_BongObstacle(SOCKET sock);