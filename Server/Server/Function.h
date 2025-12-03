#pragma once

#include"Common.h"
#include "obstacle.h"
#include <glm/gtc/matrix_transform.hpp>
#include <gl/glew.h>

extern CRITICAL_SECTION g_cs;  // 임계영역
extern int g_clientCount;
extern ClientInfo g_clients[MAX_CLIENTS];
extern bool countEnd;


bool IsAllPlayersReady();					//3명 모두 접속했니? 
void SendToAllClients(int data);			//한번에 모든 클라소켓에 카운트다운 숫자 보내기
DWORD WINAPI CountdownThread(LPVOID arg);	//카운트다운 쓰레드 함수
int S2C_ClientOrder(SOCKET sock, int order);   //클라에게 몇번째 클라인지 보내주는 함수
