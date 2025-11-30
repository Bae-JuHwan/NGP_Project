#include "clientPacketHandler.h"
#include <cstring>

// 안전하게 보내기
bool SendPacket(SOCKET sock, const void* pkt, int size) {
    int sent = 0;
    const char* ptr = (const char*)pkt;

    while (sent < size) {
        int s = send(sock, ptr + sent, size - sent, 0);
        if (s <= 0) return false;
        sent += s;
    }
    return true;
}

// 안전하게 받기
bool RecvPacket(SOCKET sock, void* buf, int size) {
    int recved = 0;
    char* ptr = (char*)buf;

    while (recved < size) {
        int r = recv(sock, ptr + recved, size - recved, 0);
        if (r <= 0) return false; // 오류 또는 연결 종료
        recved += r;
    }
    return true;
}

// 헤더 먼저 받기
bool RecvHeader(SOCKET sock, PacketHeader* header) {
    return RecvPacket(sock, header, sizeof(PacketHeader));
}

// 예시: 패킷 처리 루프
void ProcessPacket(SOCKET sock) {
    PacketHeader header;
    if (!RecvHeader(sock, &header)) return;

    switch (header.type) {
    case PACKET_S2C_CHARACTER: {
        CharacterPacket pkt;
        pkt.header = header;
        if (RecvPacket(sock, &pkt.data, sizeof(character))) {
            // 캐릭터 데이터 처리
        }
        break;
    }
    case PACKET_S2C_OBSTACLE: {
        ObstaclePacket pkt;
        pkt.header = header;
        if (RecvPacket(sock, &pkt.data, sizeof(obstacle_Bong))) {
            // 장애물 데이터 처리
        }
        break;
    }
    default:
        // 알 수 없는 패킷
        break;
    }
}