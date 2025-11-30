#pragma once
#include <winsock2.h>
#include <cstdint>
#include "obstacle.h"
#include "obstacle_network.h"

#pragma pack(1)
struct character {
    int ID;
    glm::vec3 position;
    glm::vec3 direction;
    GLfloat ArmLegSwingAngle;
    bool isCollision;
};
#pragma pack()

// 서버에서 보내는 패킷 종류
enum PacketType {
    PACKET_S2C_CHARACTER = 1,
    PACKET_S2C_OBSTACLE = 2,
};

// 패킷 헤더
struct PacketHeader {
    int type;   // PacketType
    int size;   // 전체 패킷 크기 (헤더 + 데이터)
};

// 서버 → 클라 패킷 구조
struct CharacterPacket {
    PacketHeader header;
    character data;
};

struct ObstaclePacket {
    PacketHeader header;
    obstacle_Bong data;
};

// 송수신 함수
bool SendPacket(SOCKET sock, const void* pkt, int size);
bool RecvPacket(SOCKET sock, void* buf, int size);

// 패킷 받기 예제
bool RecvHeader(SOCKET sock, PacketHeader* header);