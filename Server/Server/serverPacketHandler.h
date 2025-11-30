#pragma once
#include <cstdint>
#include "obstacle.h"

enum PacketType {
    PACKET_S2C_CHARACTER = 1,
    PACKET_S2C_OBSTACLE = 2,
};

struct PacketHeader {
    int type;   // PacketType
    int size;   // 전체 패킷 크기   
};

struct CharacterPacket {
    PacketHeader header;
    character data;
};

struct ObstaclePacket {
    PacketHeader header;
    obstacle_Bong data;
};