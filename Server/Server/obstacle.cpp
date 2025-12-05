#include "obstacle.h"
#include "stdafx.h"
#include "Common.h"

Moving_Obstacle g_bongObstacle;
Moving_Obstacle g_doorObstacle;
int sendBongCount = 0;
int sendDoorCount = 0;

void InitMovingObstacle()
{
	g_bongObstacle.type = Bong_Obstacle;
	g_bongObstacle.pos1 = glm::vec3(-1.0f, 1.0f, 0.0f);
	g_bongObstacle.dir1 = glm::vec3(1.0f, 0.0f, 0.0f);
	g_bongObstacle.pos2 = glm::vec3(1.0f, 1.0f, 0.0f);
	g_bongObstacle.dir2 = glm::vec3(-1.0f, 0.0f, 0.0f);

	g_doorObstacle.type = Door_Obstacle;
	g_doorObstacle.pos1 = glm::vec3(-1.0f, 0.0f, 0.0f);
	g_doorObstacle.dir1 = glm::vec3(1.0f, 0.0f, 0.0f);
	g_doorObstacle.pos2 = glm::vec3(1.0f, 0.0f, 0.0f);
	g_doorObstacle.dir2 = glm::vec3(-1.0f, 0.0f, 0.0f);
}

bool S2C_MovingObstacle(SOCKET sock, const Moving_Obstacle& obs_info)
{
	if (sock == INVALID_SOCKET) {
		printf("[경고] 소켓이 유효하지 않습니다\n");
		return false;
	}

	//  패킷 헤더 생성
	PacketHeader header;
	
	if (obs_info.type == Bong_Obstacle)
		header.type = PACKET_BONG_OBSTACLE;
	else if (obs_info.type == Door_Obstacle)
		header.type = PACKET_DOOR_OBSTACLE;
	else {
		printf("[경고] 알 수 없는 장애물 타입(%d)\n", obs_info.type);
		return false;
	}

	header.size = sizeof(Moving_Obstacle);

	// ===== 헤더 전송 =====
	int headerSend = send(sock, (const char*)&header, sizeof(PacketHeader), 0);
	if (headerSend == SOCKET_ERROR) {
		printf("[에러] 장애물 헤더 전송 실패\n");
		return false;
	}

	// ===== 본문(장애물 데이터) 전송 =====
	int retval = send(sock, (char*)&obs_info, sizeof(Moving_Obstacle), 0);

	// 카운트 증가
	if (obs_info.type == Bong_Obstacle)
		sendBongCount++;
	else
		sendDoorCount++;

	// ===== 디버깅 출력(100회마다) =====
	if (obs_info.type == Bong_Obstacle && sendBongCount % 100 == 0)
	{
		printf("[서버] 봉 장애물 전송 %d회\n", sendBongCount);
		printf("pos1 : x=%f y=%f z=%f\n", obs_info.pos1.x, obs_info.pos1.y, obs_info.pos1.z);
		printf("pos2 : x=%f y=%f z=%f\n", obs_info.pos2.x, obs_info.pos2.y, obs_info.pos2.z);
	}

	if (obs_info.type == Door_Obstacle && sendDoorCount % 100 == 0)
	{
		printf("[서버] 문 장애물 전송 %d회\n", sendDoorCount);
		printf("pos1 : x=%f y=%f z=%f\n", obs_info.pos1.x, obs_info.pos1.y, obs_info.pos1.z);
		printf("pos2 : x=%f y=%f z=%f\n", obs_info.pos2.x, obs_info.pos2.y, obs_info.pos2.z);
	}

	// ===== 오류 체크 =====
	if (retval == SOCKET_ERROR) {
		err_display("send() - S2C_MovingObstacle");
		return false;
	}

	if (retval != sizeof(Moving_Obstacle)) {
		printf("[경고] 전송 크기 불일치 (예상 : %zu, 실제 : %d)\n",
			sizeof(Moving_Obstacle), retval);
	}

	return true;
}

void UpdateMovingObstacle()
{
	// =====  봉 장애물 업데이트  =====
	{
		float MoveSpeed = 0.03f;
		float MaxMoveDistance = 1.6f;

		// pos1
		g_bongObstacle.pos1 += g_bongObstacle.dir1 * MoveSpeed;
		if (g_bongObstacle.pos1.x >= MaxMoveDistance) g_bongObstacle.dir1.x = -1;
		else if (g_bongObstacle.pos1.x <= -MaxMoveDistance) g_bongObstacle.dir1.x = 1;

		// pos2
		g_bongObstacle.pos2 += g_bongObstacle.dir2 * MoveSpeed;
		if (g_bongObstacle.pos2.x >= MaxMoveDistance) g_bongObstacle.dir2.x = -1;
		else if (g_bongObstacle.pos2.x <= -MaxMoveDistance) g_bongObstacle.dir2.x = 1;
	}

	// =====  문 장애물 업데이트  =====
	{
		float MoveSpeed = 0.008f;
		float MaxMoveDistance = 1.7f;

		// left door (pos1)
		g_doorObstacle.pos1 += g_doorObstacle.dir1 * MoveSpeed;
		if (g_doorObstacle.pos1.x >= 0.0f) g_doorObstacle.dir1.x = -1;
		else if (g_doorObstacle.pos1.x <= -MaxMoveDistance) g_doorObstacle.dir1.x = 1;

		// right door (pos2)
		g_doorObstacle.pos2 += g_doorObstacle.dir2 * MoveSpeed;
		if (g_doorObstacle.pos2.x >= MaxMoveDistance) g_doorObstacle.dir2.x = -1;
		else if (g_doorObstacle.pos2.x <= 0.0f) g_doorObstacle.dir2.x = 1;
	}
}