#include "InfotronObject.hpp"

const std::vector<int> InfotronObject::COLLECTING_SPRITES = {121, 122, 123, 124, 125, 126, 127};

InfotronObject::InfotronObject(int x, int y) 
    : GameObject(x, y, ObjectType::INFOTRON), collected(false), collecting(false),
      animationTimer(0.0f), frameDuration(0.04f), currentFrame(0), animating(false) {
    setSpriteId(SPRITE_INFOTRON);
}

void InfotronObject::update(float deltaTime) {
    if (animating) {
        animationTimer += deltaTime;
        
        if (animationTimer >= frameDuration) {
            animationTimer = 0.0f;
            currentFrame++;
            
            if (currentFrame >= static_cast<int>(COLLECTING_SPRITES.size())) {
                // Animation complete
                animating = false;
                collected = true;
                setActive(false);
            } else {
                setSpriteId(COLLECTING_SPRITES[currentFrame]);
            }
        }
    }
}

void InfotronObject::collect() {
    if (collected || collecting) return;
    
    collecting = true;
    animating = true;
    currentFrame = 0;
    animationTimer = 0.0f;
    setSpriteId(COLLECTING_SPRITES[0]);
}
