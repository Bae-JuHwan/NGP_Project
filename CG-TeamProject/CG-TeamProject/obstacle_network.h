#pragma once
#include <WinSock2.h>
#include"stdafx.h"

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

bool recv_BongObstacle(SOCKET sock);
bool recv_all(SOCKET sock, char* buf, int size);