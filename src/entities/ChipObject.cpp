#include "ChipObject.hpp"

ChipObject::ChipObject(int x, int y) 
    : GameObject(x, y, ObjectType::CHIP_1), collected(false) {
    setSpriteId(SPRITE_CHIP);
}

void ChipObject::update(float deltaTime) {
    // Static object for now
    // In original game, chips might have special behavior
}

void ChipObject::collect() {
    collected = true;
    setActive(false);
}
