#pragma once
#include "Common.h"

class Player {
public:
    double x, y;
    ItemType activeEffect;
    ULONGLONG effectEndTime;

    Player();
    void Move(char key);
    void Update(ULONGLONG currentTime);
    double GetSize() const;
    double GetSpeed() const;
};