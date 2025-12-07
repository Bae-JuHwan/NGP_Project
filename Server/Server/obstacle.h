#pragma once

#include <WinSock2.h>
#include "stdafx.h"
#include "Common.h"

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

#pragma pack(1)
struct Rotating_Obstacle {
    int type;
    glm::vec3 pos1;
    GLfloat angle1;
    glm::vec3 pos2;
    GLfloat angle2;
};
#pragma pack()

enum Rotating_ObstacleType {
    JumpBar_Obstacle,
    vFan_Obstacle,
    hFan_Obstacle
};

extern Moving_Obstacle g_bongObstacle;
extern Moving_Obstacle g_doorObstacle;
extern Rotating_Obstacle g_jumpbarObstacle;
extern Rotating_Obstacle g_vFanObstacle;
extern Rotating_Obstacle g_hFanObstacle;

bool S2C_MovingObstacle(SOCKET sock, const Moving_Obstacle& obs_info);
void UpdateMovingObstacle();
void InitMovingObstacle();
bool S2C_RotatingObstacle(SOCKET sock, const Rotating_Obstacle& obs_info);
void UpdateRotatingObstacle();
void InitRotatingObstacle();