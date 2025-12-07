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

#pragma pack(1)
struct Rotating_Obstacle {
    int type;
    glm::vec3 pos1;
    GLfloat angle1;
    glm::vec3 pos2;
    GLfloat angle2;
};
#pragma pack()

extern Moving_Obstacle g_bongObstacle;
extern Moving_Obstacle g_doorObstacle;
extern Rotating_Obstacle g_jumpbarObstacle;
extern Rotating_Obstacle g_verticalfanObstacle;
extern Rotating_Obstacle g_horizontalfanObstacle;

bool recv_MovingObstacle(SOCKET sock);
bool recv_RotatingObstacle(SOCKET sock);