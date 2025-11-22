#pragma once

#include <WinSock2.h>
#include <glm.hpp>
#include <gl/glew.h>

// 장애물 구조체
#pragma pack(1)
struct MovingObstacle {
    glm::vec3 position;
    glm::vec3 direction;
};

struct RotatingObstacle {
    glm::vec3 position;
    GLfloat angle;
};
#pragma pack()

extern MovingObstacle g_movingObstacle;
extern RotatingObstacle g_rotatingObstacle;

// 장애물 관련 함수 원형 선언
bool S2C_MovingObstacle(SOCKET sock, const MovingObstacle& obs_info);
bool S2C_RotatingObstacle(SOCKET sock, const RotatingObstacle& obs_info);
// void UpdateObstacleState();