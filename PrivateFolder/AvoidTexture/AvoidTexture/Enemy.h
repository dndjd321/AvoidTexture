#pragma once
#include "Common.h"

class Enemy {
public:
    double x, y, dx, dy;
    char shape;

    Enemy(double targetX, double targetY);
    bool Update(); // 화면 밖으로 나가면 true 반환
};