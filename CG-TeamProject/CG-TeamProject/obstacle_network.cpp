#include "obstacle_network.h"
#include <stdio.h>

obstacle_Bong g_bongObstacle; // 전역 변수 정의
extern CRITICAL_SECTION g_cs_client;

bool recv_BongObstacle(SOCKET sock)
{
	if (sock == INVALID_SOCKET) {
		printf("[경고] 소켓이 유효하지 않습니다\n");
		return false;
	}

	obstacle_Bong obs_info;

	// 서버로부터 장애물 정보 패킷 수신
	int retval = recv(sock, (char*)&obs_info, sizeof(obstacle_Bong), 0);
	printf("장애물 정보 받음\n");
	if (retval == SOCKET_ERROR) {
		int err = WSAGetLastError();
		if (err != WSAEWOULDBLOCK) {
			printf("[에러] recv_character() 실패 - 에러코드: %d\n", err);
			return false;
		}
		return false;  // 데이터 없음
	}

	if (retval == 0) {
		printf("[경고] 서버와의 연결이 종료되었습니다\n");
		return false;
	}

<<<<<<< HEAD
	return true;
=======

    printf("[클라] 봉 장애물 정보 수신 완료 (%d 바이트)\n", total_received);
    return true;
>>>>>>> 447595995a4c602e9bf9c232e9b4cee5b55f9baa
}