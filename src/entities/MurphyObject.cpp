#include "MurphyObject.hpp"
#include "../game/Level.hpp"
#include "../systems/AssetManager.hpp"
#include <iostream>

MurphyObject::MurphyObject(int startX, int startY) 
    : GameObject(startX, startY, ObjectType::PLAYER),
      renderX(startX), renderY(startY), targetX(startX), targetY(startY),
      moving(false), moveSpeed(MOVE_SPEED), currentFrame(0), animationTimer(0.0f),
      frameDuration(ANIMATION_SPEED), isAnimating(false), idleSprite(MURPHY_IDLE),
      pendingMoveX(0), pendingMoveY(0), facingDirection(FacingDirection::IDLE),
      isDigging(false), hasPendingObjectRemoval(false), pendingRemovalX(0), 
      pendingRemovalY(0), pendingLevel(nullptr) {
    
    setSpriteId(MURPHY_IDLE);
}

void MurphyObject::update(float deltaTime) {
    updateAnimation(deltaTime);
    updateMovement(deltaTime);
}

void MurphyObject::render(SDL_Renderer* renderer, float offsetX, float offsetY) {
    if (!active) return;
    
    int pixelX = static_cast<int>((renderX * TILE_SIZE) + offsetX);
    int pixelY = static_cast<int>((renderY * TILE_SIZE) + offsetY);
    sprite.render(renderer, pixelX, pixelY);
}

void MurphyObject::handleInput(const SDL_Event& event, Level* level) {
    // Event-based input handling if needed
}

void MurphyObject::processInput(Level* level) {
    checkContinuousInput(level);
}

void MurphyObject::checkContinuousInput(Level* level) {
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    
    bool spacePressed = keystate[SDL_SCANCODE_SPACE];
    
    pendingMoveX = 0;
    pendingMoveY = 0;
    
    if (keystate[SDL_SCANCODE_LEFT] || keystate[SDL_SCANCODE_A]) {
        pendingMoveX = -1;
        facingDirection = FacingDirection::LEFT;
        idleSprite = MURPHY_LEFT_1;
    }
    else if (keystate[SDL_SCANCODE_RIGHT] || keystate[SDL_SCANCODE_D]) {
        pendingMoveX = 1;
        facingDirection = FacingDirection::RIGHT;
        idleSprite = MURPHY_RIGHT_1;
    }
    else if (keystate[SDL_SCANCODE_UP] || keystate[SDL_SCANCODE_W]) {
        pendingMoveY = -1;
    }
    else if (keystate[SDL_SCANCODE_DOWN] || keystate[SDL_SCANCODE_S]) {
        pendingMoveY = 1;
    }
    
    if (!moving && !isDigging && (pendingMoveX != 0 || pendingMoveY != 0)) {
        if (spacePressed) {
            dig(pendingMoveX, pendingMoveY, level);
        } else {
            move(pendingMoveX, pendingMoveY, level);
        }
    }
}

void MurphyObject::dig(int dx, int dy, Level* level) {
    int targetX = x + dx;
    int targetY = y + dy;
    
    if (targetX < 0 || targetX >= Level::LEVEL_WIDTH || targetY < 0 || targetY >= Level::LEVEL_HEIGHT) {
        return;
    }
    
    level->digAt(targetX, targetY);
    
    // Set Murphy's sprite based on digging direction
    if (dx == -1) {
        setSpriteId(MURPHY_DIG_LEFT);
        facingDirection = FacingDirection::LEFT;
        idleSprite = MURPHY_LEFT_1;
    } else if (dx == 1) {
        setSpriteId(MURPHY_DIG_RIGHT);
        facingDirection = FacingDirection::RIGHT;
        idleSprite = MURPHY_RIGHT_1;
    } else if (dy == -1) {
        setSpriteId(MURPHY_DIG_UP);
    } else if (dy == 1) {
        setSpriteId(MURPHY_DIG_DOWN);
    }
}

void MurphyObject::move(int dx, int dy, Level* level) {
    int newX = x + dx;
    int newY = y + dy;
    
    if (level->isWalkable(newX, newY)) {
        GameObject* obj = level->getObjectAt(newX, newY);
        
        if (obj) {
            if (obj->getType() == ObjectType::INFOTRON) {
                InfotronObject* infoObj = static_cast<InfotronObject*>(obj);
                if (!infoObj->isCollecting()) {
                    // Only collect if not already collecting
                    level->digAt(newX, newY);
                }
            } else if (obj->getType() == ObjectType::BASE) {
                hasPendingObjectRemoval = true;
                pendingRemovalX = newX;
                pendingRemovalY = newY;
                pendingLevel = level;
            }
        }
        
        x = newX;
        y = newY;
        
        targetX = static_cast<float>(newX);
        targetY = static_cast<float>(newY);
        moving = true;
        
        if (dx == -1) {
            startAnimation({MURPHY_LEFT_1, MURPHY_LEFT_2, MURPHY_LEFT_3, MURPHY_LEFT_2, MURPHY_LEFT_1}, ANIMATION_SPEED);
        } else if (dx == 1) {
            startAnimation({MURPHY_RIGHT_1, MURPHY_RIGHT_2, MURPHY_RIGHT_3, MURPHY_RIGHT_2, MURPHY_RIGHT_1}, ANIMATION_SPEED);
        } else if (dy == -1 || dy == 1) {
            if (facingDirection == FacingDirection::LEFT) {
                startAnimation({MURPHY_LEFT_1, MURPHY_LEFT_2, MURPHY_LEFT_3, MURPHY_LEFT_2, MURPHY_LEFT_1}, ANIMATION_SPEED);
            } else {
                startAnimation({MURPHY_RIGHT_1, MURPHY_RIGHT_2, MURPHY_RIGHT_3, MURPHY_RIGHT_2, MURPHY_RIGHT_1}, ANIMATION_SPEED);
                facingDirection = FacingDirection::RIGHT;
                idleSprite = MURPHY_RIGHT_1;
            }
        }
    }
}

void MurphyObject::startAnimation(const std::vector<int>& frames, float frameDuration) {
    currentAnimationFrames = frames;
    this->frameDuration = frameDuration;
    currentFrame = 0;
    animationTimer = 0.0f;
    isAnimating = true;
    
    if (!frames.empty()) {
        setSpriteId(frames[0]);
    }
}

void MurphyObject::updateAnimation(float deltaTime) {
    if (!isAnimating || currentAnimationFrames.empty()) {
        return;
    }
    
    animationTimer += deltaTime;
    
    if (animationTimer >= frameDuration) {
        animationTimer = 0.0f;
        currentFrame++;
        
        if (currentFrame >= static_cast<int>(currentAnimationFrames.size())) {
            if (isDigging) {
                isDigging = false;
                isAnimating = false;
                setSpriteId(idleSprite);
            } else {
                currentFrame = 0;
                
                const Uint8* keystate = SDL_GetKeyboardState(NULL);
                bool anyMovementKey = keystate[SDL_SCANCODE_LEFT] || keystate[SDL_SCANCODE_A] ||
                                     keystate[SDL_SCANCODE_RIGHT] || keystate[SDL_SCANCODE_D] ||
                                     keystate[SDL_SCANCODE_UP] || keystate[SDL_SCANCODE_W] ||
                                     keystate[SDL_SCANCODE_DOWN] || keystate[SDL_SCANCODE_S];
                
                if (anyMovementKey) {
                    setSpriteId(currentAnimationFrames[currentFrame]);
                } else {
                    isAnimating = false;
                    setSpriteId(idleSprite);
                }
            }
        } else {
            setSpriteId(currentAnimationFrames[currentFrame]);
        }
    }
}

void MurphyObject::updateMovement(float deltaTime) {
    if (!moving) {
        return;
    }
    
    float dx = targetX - renderX;
    float dy = targetY - renderY;
    float moveDistance = moveSpeed * deltaTime;
    float distanceToTarget = sqrt(dx * dx + dy * dy);
    
    if (distanceToTarget <= moveDistance) {
        renderX = targetX;
        renderY = targetY;
        moving = false;
        
        if (hasPendingObjectRemoval && pendingLevel) {
            pendingLevel->removeObjectAt(pendingRemovalX, pendingRemovalY);
            hasPendingObjectRemoval = false;
            pendingLevel = nullptr;
        }
    } else {
        float normalizedDx = dx / distanceToTarget;
        float normalizedDy = dy / distanceToTarget;
        
        renderX += normalizedDx * moveDistance;
        renderY += normalizedDy * moveDistance;
    }
}
