#pragma once
#include <algorithm>
#include"stdafx.h"
// 전방 선언
class Player1;

// 두 AABB 충돌 검사
bool checkCollision(Player1* p1, Player1* p2);

// 두 플레이어 간 충돌 처리
void resolveCollision(Player1* p1, Player1* p2);

// 모든 플레이어 간 충돌 처리
void handleAllPlayerCollisions(Player1* P1, Player1* P2, Player1* P3);