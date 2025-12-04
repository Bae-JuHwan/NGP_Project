#include "collision.h"
#include "Charactor.h"  // Player1 클래스 헤더

// 두 AABB 충돌 검사
bool checkCollision(Player1* p1, Player1* p2) {
    if (p1 == nullptr || p2 == nullptr) return false;

    return (p1->CAABB.min.x <= p2->CAABB.max.x && p1->CAABB.max.x >= p2->CAABB.min.x) &&
        (p1->CAABB.min.y <= p2->CAABB.max.y && p1->CAABB.max.y >= p2->CAABB.min.y) &&
        (p1->CAABB.min.z <= p2->CAABB.max.z && p1->CAABB.max.z >= p2->CAABB.min.z);
}

// 두 플레이어 간 충돌 처리
void resolveCollision(Player1* p1, Player1* p2) {
    if (p1 == nullptr || p2 == nullptr) return;
    if (!checkCollision(p1, p2)) return;

    // 겹침 계산
    float overlapX = std::min(p1->CAABB.max.x, p2->CAABB.max.x) - std::max(p1->CAABB.min.x, p2->CAABB.min.x);
    float overlapZ = std::min(p1->CAABB.max.z, p2->CAABB.max.z) - std::max(p1->CAABB.min.z, p2->CAABB.min.z);

    // 최소 겹침 축으로 분리
    if (overlapX < overlapZ) {
        // X축으로 분리
        float separation = overlapX / 2.0f;
        if (p1->Position.x < p2->Position.x) {
            p1->Position.x -= separation;
            p2->Position.x += separation;
        }
        else {
            p1->Position.x += separation;
            p2->Position.x -= separation;
        }

        // 이동 방향 차단
        p1->Direction.x = 0.0f;
        p2->Direction.x = 0.0f;
    }
    else {
        // Z축으로 분리
        float separation = overlapZ / 2.0f;
        if (p1->Position.z < p2->Position.z) {
            p1->Position.z -= separation;
            p2->Position.z += separation;
        }
        else {
            p1->Position.z += separation;
            p2->Position.z -= separation;
        }

        // 이동 방향 차단
        p1->Direction.z = 0.0f;
        p2->Direction.z = 0.0f;
    }
}

// 모든 플레이어 간 충돌 처리
void handleAllPlayerCollisions(Player1* P1, Player1* P2, Player1* P3) {
    resolveCollision(P1, P2);
    resolveCollision(P1, P3);
    resolveCollision(P2, P3);
}