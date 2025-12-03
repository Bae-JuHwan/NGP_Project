#include "obstacle.h"
#include "stdafx.h"
#include "Common.h"

Bong_Obstacle g_bongObstacle;
Door_Obstacle g_doorObstacle;
int sendBongCount = 0;
int sendDoorCount = 0;

void InitBongObstacle()
{
	g_bongObstacle.pos1 = glm::vec3(-1.0f, 1.0f, 0.0f);
	g_bongObstacle.dir1 = glm::vec3(1.0f, 0.0f, 0.0f);
	g_bongObstacle.pos2 = glm::vec3(1.0f, 1.0f, 0.0f);
	g_bongObstacle.dir2 = glm::vec3(-1.0f, 0.0f, 0.0f);

}

void InitDoorObstacle()
{
	g_doorObstacle.pos1 = glm::vec3(-1.0f, 0.0f, 0.0f);
	g_doorObstacle.dir1 = glm::vec3(1.0f, 0.0f, 0.0f);
	g_doorObstacle.pos2 = glm::vec3(1.0f, 0.0f, 0.0f);
	g_doorObstacle.dir2 = glm::vec3(-1.0f, 0.0f, 0.0f);
}

bool S2C_BongObstacle(SOCKET sock, const Bong_Obstacle& obs_info)
{
	if (sock == INVALID_SOCKET) {
		printf("[경고] 소켓이 유효하지 않습니다\n");
		return false;
	}

	//  패킷 헤더 생성
	PacketHeader header;
	header.type = PACKET_BONG_OBSTACLE;
	header.size = sizeof(Bong_Obstacle);

	// 헤더 전송
	if (!send(sock, (const char*)&header, sizeof(PacketHeader),0)) {
		printf("[에러] 봉 장애물 헤더 전송 실패\n");
		return false;
	}

	int retval = send(sock, (char*)&obs_info, sizeof(Bong_Obstacle), 0);
	sendBongCount++;
	if(sendBongCount % 100 ==0)
	{
		printf("[서버] 봉 장애물 정보 전송 완료 송신 %d회 \n", sendBongCount);
		printf("봉 장애물 위치1 : x=%f, y=%f, z=%f\n", obs_info.pos1.x, obs_info.pos1.y, obs_info.pos1.z);
		printf("봉 장애물 위치2 : x=%f, y=%f, z=%f\n", obs_info.pos2.x, obs_info.pos2.y, obs_info.pos2.z);
	}

	if (retval == SOCKET_ERROR) {
		err_display("send() - S2C_MovingObstacle");
		return false;
	}

	if (retval != sizeof(Bong_Obstacle)) {
		printf("[경고] 봉 장애물 전송 크기 불일치 (예상 : %zu, 실제 : %d)\n", sizeof(Bong_Obstacle), retval);
	}

	return true;
}

bool S2C_DoorObstacle(SOCKET sock, const Door_Obstacle& obs_info)
{
	if (sock == INVALID_SOCKET) {
		printf("[경고] 소켓이 유효하지 않습니다\n");
		return false;
	}

	//  패킷 헤더 생성
	PacketHeader header;
	header.type = PACKET_DOOR_OBSTACLE;
	header.size = sizeof(Door_Obstacle);

	// 헤더 전송
	if (!send(sock, (const char*)&header, sizeof(PacketHeader), 0)) {
		printf("[에러] 봉 장애물 헤더 전송 실패\n");
		return false;
	}

	int retval = send(sock, (char*)&obs_info, sizeof(Door_Obstacle), 0);
	sendDoorCount++;
	if (sendDoorCount % 100 == 0)
	{
		printf("[서버] 봉 장애물 정보 전송 완료 송신 %d회 \n", sendDoorCount);
		printf("문 장애물 위치1 : x=%f, y=%f, z=%f\n", obs_info.pos1.x, obs_info.pos1.y, obs_info.pos1.z);
		printf("문 장애물 위치2 : x=%f, y=%f, z=%f\n", obs_info.pos2.x, obs_info.pos2.y, obs_info.pos2.z);
	}

	if (retval == SOCKET_ERROR) {
		err_display("send() - S2C_MovingObstacle");
		return false;
	}

	if (retval != sizeof(Door_Obstacle)) {
		printf("[경고] 봉 장애물 전송 크기 불일치 (예상 : %zu, 실제 : %d)\n", sizeof(Door_Obstacle), retval);
	}

	return true;
}

void UpdateBongObstacle() {
	float MoveSpeed = 0.03f; 
	float MaxMoveDistance = 1.6f;

	g_bongObstacle.pos1 += g_bongObstacle.dir1 * MoveSpeed; 
	if (g_bongObstacle.pos1.x >= MaxMoveDistance) g_bongObstacle.dir1.x = -1;
	else if (g_bongObstacle.pos1.x <= -MaxMoveDistance) g_bongObstacle.dir1.x = 1;

	g_bongObstacle.pos2 += g_bongObstacle.dir2 * MoveSpeed; 
	if (g_bongObstacle.pos2.x >= MaxMoveDistance) g_bongObstacle.dir2.x = -1;
	else if (g_bongObstacle.pos2.x <= -MaxMoveDistance) g_bongObstacle.dir2.x = 1;
}

void UpdateDoorObstacle()
{
	float MoveSpeed = 0.008f;
	float MaxMoveDistance = 1.7f;

	// left
	g_doorObstacle.pos1 += g_doorObstacle.dir1 * MoveSpeed;   // pos1 = LeftDoor
	if (g_doorObstacle.pos1.x >= 0.0f) g_doorObstacle.dir1.x = -1;
	else if (g_doorObstacle.pos1.x <= -MaxMoveDistance) g_doorObstacle.dir1.x = 1;

	// right
	g_doorObstacle.pos2 += g_doorObstacle.dir2 * MoveSpeed;   // pos2 = RightDoor
	if (g_doorObstacle.pos2.x >= MaxMoveDistance) g_doorObstacle.dir2.x = -1;
	else if (g_doorObstacle.pos2.x <= 0.0f) g_doorObstacle.dir2.x = 1;
}

//bool Broadcast_BongObstacle(const Moving_Obstacle& obs_info , ClientInfo g_clients[MAX_CLIENTS])
//{
//	for (int i = 0; i < MAX_CLIENTS; i++) {
//		if (g_clients[i].isActive) {
//			S2C_MovingObstacle(g_clients[i].sock, obs_info);
//		}
//		else {
//			std::cout << i << "번 째 클라이언트 봉 정보 송신 실패" << "\n";
//		}
//	}
//	return true;
//}