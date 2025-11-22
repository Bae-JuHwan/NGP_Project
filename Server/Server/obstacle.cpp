#include "obstacle.h"
#include "Common.h"

MovingObstacle g_movingObstacle = { glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f) };
RotatingObstacle g_rotatingObstacle = { glm::vec3(0.0f, 0.0f, 0.0f), 0.0f };

bool S2C_MovingObstacle(SOCKET sock, const MovingObstacle& obs_info) {
    if (sock == INVALID_SOCKET) {
        printf("[경고] 소켓이 유효하지 않습니다\n");
        return false;
    }

    // 클라한테 장애물 정보 전송
    int retval = send(sock, (char*)&obs_info, sizeof(MovingObstacle), 0);

    if (retval == SOCKET_ERROR) {
        err_display("send() - S2C_MovingObstacle");
        return false;
    }

    if (retval != sizeof(MovingObstacle)) {
        printf("[경고] 전송된 데이터 크기 불일치 (예상 : %zu, 실제 : %d)\n", sizeof(MovingObstacle), retval);
    }

    return true;
}

bool S2C_RotatingObstacle(SOCKET sock, const RotatingObstacle& obs_info) {
    if (sock == INVALID_SOCKET) {
        printf("[경고] 소켓이 유효하지 않습니다\n");
        return false;
    }

    // 클라한테 장애물 정보 전송
    int retval = send(sock, (char*)&obs_info, sizeof(RotatingObstacle), 0);

    if (retval == SOCKET_ERROR) {
        err_display("send() - S2C_RotatingObstacle");
        return false;
    }

    if (retval != sizeof(RotatingObstacle)) {
        printf("[경고] 전송된 데이터 크기 불일치 (예상 : %zu, 실제 : %d)\n", sizeof(RotatingObstacle), retval);
    }

    return true;
}