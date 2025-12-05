#pragma once

#include <WinSock2.h>
#include "stdafx.h"
#include "Common.h"
#include "stdafx.h"

// 서버 -> 클라 첫번째 장애물(봉) 보내기 구조체
#pragma pack(1)
struct Moving_Obstacle {
    int type;
    glm::vec3 pos1;
    glm::vec3 dir1;
    glm::vec3 pos2;
    glm::vec3 dir2;
};
#pragma pack()

enum Moving_ObstacleType {
    Bong_Obstacle,
    Door_Obstacle
};

extern Moving_Obstacle g_bongObstacle;
extern Moving_Obstacle g_doorObstacle;

bool S2C_MovingObstacle(SOCKET sock, const Moving_Obstacle& obs_info);
void UpdateMovingObstacle();
void InitMovingObstacle();