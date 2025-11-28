#include "BaseObject.hpp"

const std::vector<int> BaseObject::DIGGING_SPRITES = {40, 41, 42, 43, 44};

BaseObject::BaseObject(int x, int y) 
    : GameObject(x, y, ObjectType::BASE), digging(false), animationTimer(0.0f), 
      frameDuration(0.04f), currentFrame(0), animating(false) {  // Change this value to adjust speed
    setSpriteId(SPRITE_BASE);
}

void BaseObject::update(float deltaTime) {
    if (animating) {
        animationTimer += deltaTime;
        
        if (animationTimer >= frameDuration) {
            animationTimer = 0.0f;
            currentFrame++;
            
            if (currentFrame >= static_cast<int>(DIGGING_SPRITES.size())) {
                // Animation complete
                animating = false;
                setActive(false);
            } else {
                setSpriteId(DIGGING_SPRITES[currentFrame]);
            }
        }
    }
}

void BaseObject::startDigging() {
    if (digging) return;
    
    digging = true;
    animating = true;
    currentFrame = 0;
    animationTimer = 0.0f;
    setSpriteId(DIGGING_SPRITES[0]);
}
