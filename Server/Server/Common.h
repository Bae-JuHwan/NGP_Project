#pragma once
#define _CRT_SECURE_NO_WARNINGS // 구형 C 함수 사용 시 경고 끄기
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 구형 소켓 API 사용 시 경고 끄기
#define MAX_CLIENTS 3

#include <winsock2.h> // 윈속2 메인 헤더
#include <ws2tcpip.h> // 윈속2 확장 헤더

#include <tchar.h> // _T(), ...
#include <stdio.h> // printf(), ...
#include <stdlib.h> // exit(), ...
#include <string.h> // strncpy(), ...
#include<iostream>
#include "stdafx.h"

#pragma comment(lib, "ws2_32") // ws2_32.lib 링크

// 캐릭터 구조체
// 패킷 타입 정의
enum PacketType {
    PACKET_CHARACTER = 1,
    PACKET_BONG_OBSTACLE = 2,
    PACKET_DOOR_OBSTACLE = 3,
    PACKET_JUMPBAR_OBSTACLE = 4,
    PACKET_VFAN_OBSTACLE = 5,
    PACKET_HFAN_OBSTACLE = 6,
};

// 패킷 헤더 구조체
#pragma pack(1)
struct PacketHeader {
    int type;      // 패킷 종류
    int size;      // 데이터 크기
};
#pragma pack()


// 캐릭터 구조체
#pragma pack(1)
struct character {
    int ID;
    glm::vec3 position;
    glm::vec3 direction;
    GLfloat RotationAngle;
    GLfloat ArmLegSwingAngle;
    bool isCollision;
};
#pragma pack()


// 클라이언트 정보 구조체 추가
struct ClientInfo {
    SOCKET sock;
    int id;
    character charInfo;
    bool isActive;
};


// 캐릭터 정보 저장하기 위해서 클라이언트 정보 구조체 배열
extern ClientInfo g_clients[MAX_CLIENTS];

void err_quit(const char* msg);

void err_display(const char* msg);

void err_display(int errcode);