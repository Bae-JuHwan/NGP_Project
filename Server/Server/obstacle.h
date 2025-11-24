#pragma once

#include <WinSock2.h>
#include <glm.hpp>
#include <gl/glew.h>

// 장애물 구조체
#pragma pack(1)

// 서버 -> 클라 첫번째 장애물(봉) 보내기 구조체
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
bool Broadcast_BongObstacle(const obstacle_Bong& obs_info);