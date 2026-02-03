#include "Enemy.h"

Enemy::Enemy(double targetX, double targetY) {
    // 1. 생성 위치 결정 (상, 하, 좌, 우)
    int side = rand() % 4;
    if (side == 0) { x = rand() % WIDTH; y = 0; }
    else if (side == 1) { x = rand() % WIDTH; y = HEIGHT - 1; }
    else if (side == 2) { x = 0; y = rand() % HEIGHT; }
    else { x = WIDTH - 1; y = rand() % HEIGHT; }

    // 2. 속도 설정 (여기서 숫자만 바꾸면 전체적인 장애물 속도가 바뀝니다)
    // 0.5는 느릿함, 1.0은 보통, 1.5 이상은 꽤 빠름
    double speedMultiplier = 0.4;

    // 3. 방향 및 속도 계산
    double angle = atan2(targetY - y, targetX - x);
    dx = cos(angle) * speedMultiplier;
    dy = sin(angle) * speedMultiplier;

    // 4. 랜덤 모양 결정
    char shapes[] = { 'O', 'D', 'V' };
    shape = shapes[rand() % 3];
}

bool Enemy::Update() {
    // 매 프레임마다 계산된 dx, dy만큼 이동
    x += dx;
    y += dy;

    // 화면 경계를 벗어났는지 확인
    return (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT);
}