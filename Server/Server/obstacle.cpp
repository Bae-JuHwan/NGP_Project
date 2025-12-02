#include "obstacle.h"
#include "stdafx.h"
#include "Common.h"
// 원래꺼
obstacle_Bong g_bongObstacle;
int sendBongCount = 0;

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

	// 헤더 전송
	if (!send(sock, (const char*)&header, sizeof(PacketHeader),0)) {
		printf("[에러] 봉 장애물 헤더 전송 실패\n");
		return false;
	}



	int retval = send(sock, (char*)&obs_info, sizeof(obstacle_Bong), 0);
	sendBongCount++;
	if(sendBongCount % 100 ==0)
	{
		printf("[서버] 봉 장애물 정보 전송 완료 송신 %d회 \n", sendBongCount);
		printf("봉 장애물 위치1 : x=%f, y=%f, z=%f\n", obs_info.pos1.x, obs_info.pos1.y, obs_info.pos1.z);
		printf("봉 장애물 위치2 : x=%f, y=%f, z=%f\n", obs_info.pos2.x, obs_info.pos2.y, obs_info.pos2.z);
	}
	if (retval == SOCKET_ERROR) {
		err_display("send() - S2C_BongObstacle");
		return false;
	}

	if (retval != sizeof(obstacle_Bong)) {
		printf("[경고] 봉 장애물 전송 크기 불일치 (예상 : %zu, 실제 : %d)\n", sizeof(obstacle_Bong), retval);
	}

	return true;
}

void UpdateBongObstacle()
{
	float MoveSpeed = 0.1f;
	float MaxMoveDistance = 1.6f;

	// 봉 그룹1
	g_bongObstacle.pos1 += g_bongObstacle.dir1 * MoveSpeed;

	if (g_bongObstacle.pos1.x >= MaxMoveDistance)
		g_bongObstacle.dir1.x = -1;
	else if (g_bongObstacle.pos1.x <= -MaxMoveDistance)
		g_bongObstacle.dir1.x = 1;

	// 봉 그룹2
	g_bongObstacle.pos2 += g_bongObstacle.dir2 * MoveSpeed;

	if (g_bongObstacle.pos2.x >= MaxMoveDistance)
		g_bongObstacle.dir2.x = -1;
	else if (g_bongObstacle.pos2.x <= -MaxMoveDistance)
		g_bongObstacle.dir2.x = 1;
}

bool Broadcast_BongObstacle(const obstacle_Bong& obs_info , ClientInfo g_clients[MAX_CLIENTS])
{
	for (int i = 0; i < MAX_CLIENTS; i++) {
		if (g_clients[i].isActive) {
			S2C_BongObstacle(g_clients[i].sock, obs_info);
		}
		else {
			std::cout << i << "번 째 클라이언트 봉 정보 송신 실패" << "\n";
		}
	}
	return true;
}