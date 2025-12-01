#pragma once

#include"Common.h"
#include "obstacle.h"
#include <glm/gtc/matrix_transform.hpp>
#include <gl/glew.h>

extern CRITICAL_SECTION g_cs;  // 임계영역
extern int g_clientCount;
extern ClientInfo g_clients[MAX_CLIENTS];

bool IsAllPlayersReady();    //3명 모두 접속했니?    

bool S2C_isPlayerReady(SOCKET sock, int num);

void SendToAllClients(int data);

DWORD WINAPI CountdownThread(LPVOID arg);