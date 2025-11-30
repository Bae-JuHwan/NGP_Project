#include "obstacle_network.h"
#include "clientPacketHandler.h"
#include <stdio.h>

obstacle_Bong g_bongObstacle; // 전역 변수 정의
extern CRITICAL_SECTION g_cs_client;
int reciveBongCount = 0;

bool recv_BongObstacle(SOCKET sock)
{
	if (sock == INVALID_SOCKET) {
		printf("[경고] 소켓이 유효하지 않습니다\n");
		return false;
	}

	PacketHeader header;
	if (!RecvPacket(sock, &header, sizeof(PacketHeader))) {
		return false;
	}

	if (header.type != PACKET_S2C_OBSTACLE) {
		printf("[경고] 장애물이 다른 놈 받고 있음 : %d\n", header.type);

		char dummy[1024];
		int remaining = header.size - sizeof(PacketHeader);
		while (remaining > 0) {
			int chunk = remaining > 1024 ? 1024 : remaining;
			if (!RecvPacket(sock, dummy, chunk)) return false;
			remaining -= chunk;
		}
		return false;
	}

	ObstaclePacket pkt;
	pkt.header = header;
	if (!RecvPacket(sock, &pkt.data, sizeof(obstacle_Bong))) {
		return false;
	}

	EnterCriticalSection(&g_cs_client);
	g_bongObstacle = pkt.data;
	LeaveCriticalSection(&g_cs_client);

	reciveBongCount++;
	if (reciveBongCount % 100 == 0) {
		printf("장애물 정보 받음\n");
		printf("pos1: (%.2f, %.2f, %.2f)\n", pkt.data.pos1.x, pkt.data.pos1.y, pkt.data.pos1.z);
		printf("dir1: (%.2f, %.2f, %.2f)\n", pkt.data.dir1.x, pkt.data.dir1.y, pkt.data.dir1.z);
		printf("pos2: (%.2f, %.2f, %.2f)\n", pkt.data.pos2.x, pkt.data.pos2.y, pkt.data.pos2.z);
		printf("dir2: (%.2f, %.2f, %.2f)\n", pkt.data.dir2.x, pkt.data.dir2.y, pkt.data.dir2.z);
	}
	
    return true;
}