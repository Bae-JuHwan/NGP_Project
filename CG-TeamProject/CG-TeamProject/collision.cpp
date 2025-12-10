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

extern bool movestart;
// 모든 플레이어 간 충돌 처리
void handleAllPlayerCollisions(Player1* P1, Player1* P2, Player1* P3) {
    if (movestart) {
        resolveCollision(P1, P2);
        resolveCollision(P1, P3);
        resolveCollision(P2, P3);
    }
}

// 플레이어와 장애물 간 충돌 검사
bool checkCollision(Player1* p, const AABB& obstacleBox) {
    if (p == nullptr) return false;

    return (p->CAABB.min.x <= obstacleBox.max.x && p->CAABB.max.x >= obstacleBox.min.x) &&
        (p->CAABB.min.y <= obstacleBox.max.y && p->CAABB.max.y >= obstacleBox.min.y) &&
        (p->CAABB.min.z <= obstacleBox.max.z && p->CAABB.max.z >= obstacleBox.min.z);
}

// 플레이어와 장애물 간 충돌 해결 
void resolveCollision(Player1* p, const AABB& obstacleBox) {
    if (p == nullptr) return;
    if (!checkCollision(p, obstacleBox)) return;

    // 겹침 정도(Overlap) 계산
    float overlapX = std::min(p->CAABB.max.x, obstacleBox.max.x) - std::max(p->CAABB.min.x, obstacleBox.min.x);
    float overlapZ = std::min(p->CAABB.max.z, obstacleBox.max.z) - std::max(p->CAABB.min.z, obstacleBox.min.z);

    // Y축 겹침은 점프/착지 처리에 가까우므로, 벽 타기 방지를 위해 X, Z 위주로 처리
    // (필요하다면 Y축도 추가 가능하지만, 보통 벽/장애물은 X/Z 밀어내기로 처리함)

    // 더 적게 겹친 축으로 밀어냅니다.
    if (overlapX < overlapZ) {
        // X축 충돌 해결
        if (p->Position.x < obstacleBox.min.x) {
            // 캐릭터가 장애물 왼쪽에 있음 -> 더 왼쪽으로 밀기
            p->Position.x -= overlapX;
        }
        else {
            // 캐릭터가 장애물 오른쪽에 있음 -> 더 오른쪽으로 밀기
            p->Position.x += overlapX;
        }
        p->Direction.x = 0.0f; // 관성 제거
    }
    else {
        // Z축 충돌 해결
        if (p->Position.z < obstacleBox.min.z) {
            // 캐릭터가 장애물 앞쪽(Z값이 작은 쪽)에 있음
            p->Position.z -= overlapZ;
        }
        else {
            // 캐릭터가 장애물 뒤쪽(Z값이 큰 쪽)에 있음
            p->Position.z += overlapZ;
        }
        p->Direction.z = 0.0f; // 관성 제거
    }

    // 위치가 변경되었으므로 AABB 즉시 갱신
    p->CAABB.update(p->Position, glm::vec3(-0.7f, 0.0f, -0.72f), glm::vec3(0.7f, 1.84f, 0.63f));
}

void resolveRotatedCollision(Player1* p, glm::vec3 obsPos, glm::vec3 obsSize, float rotationAngle, glm::vec3 rotationAxis) {
    if (p == nullptr) return;

    //  플레이어의 중심 위치 계산 
    glm::vec3 playerCenter = (p->CAABB.min + p->CAABB.max) * 0.5f;
    glm::vec3 playerSize = (p->CAABB.max - p->CAABB.min) * 0.5f; // 플레이어의 반너비(Half Extents)

    // 2월드 좌표 -> 장애물 로컬 좌표 변환 행렬 생성
    glm::mat4 worldToLocal = glm::mat4(1.0f);
    worldToLocal = glm::rotate(worldToLocal, glm::radians(-rotationAngle), rotationAxis);
    worldToLocal = glm::translate(worldToLocal, -obsPos);

    // 3. 플레이어 중심을 장애물 로컬 공간으로 변환
    glm::vec4 localPC4 = worldToLocal * glm::vec4(playerCenter, 1.0f);
    glm::vec3 localPlayerCenter = glm::vec3(localPC4);

    // 4로컬 공간에서의 AABB 충돌 검사


    // 두 AABB 사이의 거리 벡터
    glm::vec3 difference = localPlayerCenter; // 장애물 중심이 (0,0,0)이므로
    glm::vec3 clamped = glm::clamp(difference, -obsSize, obsSize);
    glm::vec3 closest = clamped; // 장애물 표면상에서 플레이어와 가장 가까운 점

    difference = closest - localPlayerCenter;



    glm::vec3 minDist = -obsSize - playerSize;
    glm::vec3 maxDist = obsSize + playerSize;

    // 로컬 공간에서의 겹침 정도 계산
    float overlapX = std::min(maxDist.x, localPlayerCenter.x + playerSize.x) - std::max(minDist.x, localPlayerCenter.x - playerSize.x);
    float overlapY = std::min(maxDist.y, localPlayerCenter.y + playerSize.y) - std::max(minDist.y, localPlayerCenter.y - playerSize.y);
    float overlapZ = std::min(maxDist.z, localPlayerCenter.z + playerSize.z) - std::max(minDist.z, localPlayerCenter.z - playerSize.z);

    // 충돌이 없으면 리턴
    if (overlapX <= 0 || overlapY <= 0 || overlapZ <= 0) return;

    //  충돌 해결

    glm::vec3 separation(0.0f);

    if (overlapX < overlapZ) {
        if (localPlayerCenter.x > 0) separation.x = overlapX;
        else separation.x = -overlapX;
    }
    else {
        if (localPlayerCenter.z > 0) separation.z = overlapZ;
        else separation.z = -overlapZ;
    }

    //  로컬 분리 벡터를 다시 월드 좌표로 변환
    glm::mat4 localToWorld = glm::mat4(1.0f);
    localToWorld = glm::translate(localToWorld, obsPos);
    localToWorld = glm::rotate(localToWorld, glm::radians(rotationAngle), rotationAxis);

    // 벡터이므로 회전만 적용 (평행이동 제외) -> 0.0f
    glm::mat4 rotationOnly = glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngle), rotationAxis);
    glm::vec3 worldSeparation = glm::vec3(rotationOnly * glm::vec4(separation, 0.0f));

    // 플레이어 밀어내기
    p->Position += worldSeparation;

    // 속도/방향 제거 (미끄러짐 방지)
    // 여기서는 간단하게 멈추지만, 필요하면 미끄러지게 투영 벡터 계산 가능
    p->Direction = glm::vec3(0.0f);

    // AABB 갱신
    p->CAABB.update(p->Position, glm::vec3(-0.7f, 0.0f, -0.72f), glm::vec3(0.7f, 1.84f, 0.63f));
}