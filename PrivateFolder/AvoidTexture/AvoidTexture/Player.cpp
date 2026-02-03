#include "Player.h"

Player::Player() : x(WIDTH / 2), y(HEIGHT / 2), activeEffect(NONE), effectEndTime(0) {}

void Player::Update(ULONGLONG currentTime) {
    if (activeEffect != NONE && currentTime > effectEndTime) {
        activeEffect = NONE;
    }
}

double Player::GetSpeed() const {
    if (activeEffect == SPEED_UP) return 1.5;
    if (activeEffect == SPEED_DOWN) return 0.5;
    return 1.0;
}

double Player::GetSize() const {
    if (activeEffect == SIZE_UP) return 1.2;
    if (activeEffect == SIZE_DOWN) return 0.8;
    return 1.0;
}

void Player::Move(char key) {
    double s = GetSpeed();

    // 방향키 처리 (main에서 224 예외처리를 거친 후 들어온 key값)
    switch (key) {
    case LEFT:  if (x > 0) x -= s; break;
    case RIGHT: if (x < WIDTH - 1) x += s; break;
    case UP:    if (y > 0) y -= s; break;
    case DOWN:  if (y < HEIGHT - 1) y += s; break;
    }
}