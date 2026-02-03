#include "Item.h"

Item::Item() : x(0), y(0), type(NONE), active(false) {}

void Item::Spawn() {
    x = rand() % (WIDTH - 2) + 1;
    y = rand() % (HEIGHT - 2) + 1;
    type = (ItemType)(rand() % 4);
    active = true;
}