#pragma once
#include "Common.h"

class Item {
public:
    int x, y;
    ItemType type;
    bool active;

    Item();
    void Spawn();
};