#include "obstacle.h"
#include "stdafx.h"
#include "Common.h"

extern CRITICAL_SECTION g_cs;

obstacle_Bong g_bongObstacle;
int sendBongCount = 0;

static int send_all(SOCKET s, const char* buf, int len)
{
	int total = 0;
	while (total < len) {
		int sent = send(s, buf + total, len - total, 0);
		if (sent == SOCKET_ERROR) return SOCKET_ERROR;
		if (sent == 0) break;
		total += sent;
	}
	return total;
}

bool S2C_BongObstacle(SOCKET sock, const obstacle_Bong& obs_info)
{
	if (sock == INVALID_SOCKET) {
		printf("[경고] 소켓이 유효하지 않습니다\n");
		return false;
	}

	//  패킷 헤더 생성
	PacketHeader header;
	header.type = PACKET_OBSTACLE;
	header.size = sizeof(obstacle_Bong);

	int hsent = send_all(sock, (const char*)&header, sizeof(header));
	if (hsent == SOCKET_ERROR) {
		err_display("send() - S2C_BongObstacle header");
		return false;
	}
	if (hsent != sizeof(header)) {
		printf("[경고] 봉 장애물 헤더 전송 크기 불일치 (예상:%zu 실제:%d)\n", sizeof(header), hsent);
		return false;
	}

	// 본문 전송
	int bsent = send_all(sock, (const char*)&obs_info, sizeof(obstacle_Bong));
	if (bsent == SOCKET_ERROR) {
		err_display("send() - S2C_BongObstacle body");
		return false;
	}
	if (bsent != sizeof(obstacle_Bong)) {
		printf("[경고] 봉 장애물 전송 크기 불일치 (예상:%zu 실제:%d)\n", sizeof(obstacle_Bong), bsent);
		return false;
	}

	sendBongCount++;
	if (sendBongCount % 100 == 0) {
		printf("[서버] 봉 장애물 정보 전송 완료 송신 %d회 \n", sendBongCount);
		printf("봉 장애물 위치1 : x=%f, y=%f, z=%f\n", obs_info.pos1.x, obs_info.pos1.y, obs_info.pos1.z);
		printf("봉 장애물 위치2 : x=%f, y=%f, z=%f\n", obs_info.pos2.x, obs_info.pos2.y, obs_info.pos2.z);
	}

	return true;
}

void UpdateBongObstacle()
{
	float MoveSpeed = 0.1f;
	float MaxMoveDistance = 1.6f;

	g_bongObstacle.pos1 += g_bongObstacle.dir1 * MoveSpeed;
	if (g_bongObstacle.pos1.x >= MaxMoveDistance) g_bongObstacle.dir1.x = -1;
	else if (g_bongObstacle.pos1.x <= -MaxMoveDistance) g_bongObstacle.dir1.x = 1;

	g_bongObstacle.pos2 += g_bongObstacle.dir2 * MoveSpeed;
	if (g_bongObstacle.pos2.x >= MaxMoveDistance) g_bongObstacle.dir2.x = -1;
	else if (g_bongObstacle.pos2.x <= -MaxMoveDistance) g_bongObstacle.dir2.x = 1;
}

bool Broadcast_BongObstacle(const obstacle_Bong& obs_info, ClientInfo g_clients[MAX_CLIENTS])
{
	SOCKET targets[MAX_CLIENTS];
	int targetCount = 0;

	EnterCriticalSection(&g_cs);
	for (int i = 0; i < MAX_CLIENTS; ++i) {
		if (g_clients[i].isActive && g_clients[i].sock != INVALID_SOCKET) {
			targets[targetCount++] = g_clients[i].sock;
		}
	}
	LeaveCriticalSection(&g_cs);

	for (int i = 0; i < targetCount; ++i) {
		SOCKET s = targets[i];
		if (!S2C_BongObstacle(s, obs_info)) {
			printf("[경고] 봉 장애물 전송 실패 (소켓 인덱스 스냅샷 엔트리 %d)\n", i);
		}
	}

	return true;
}