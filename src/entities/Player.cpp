#include "Player.hpp"
#include "../game/Level.hpp"
#include "../systems/AssetManager.hpp"
#include <iostream>

Player::Player(int startX, int startY) 
    : x(startX), y(startY), renderX(startX), renderY(startY), 
      targetX(startX), targetY(startY), moving(false), moveSpeed(MOVE_SPEED),
      currentFrame(0), animationTimer(0.0f), 
      frameDuration(ANIMATION_SPEED), isAnimating(false), idleSprite(MURPHY_IDLE),
      pendingMoveX(0), pendingMoveY(0), facingDirection(FacingDirection::IDLE) {
    // Get the sprite texture and set Murphy's idle sprite
    SDL_Texture* spriteTexture = AssetManager::getInstance().getTexture("sprites");
    sprite = Sprite(spriteTexture, MURPHY_IDLE);
}

Player::~Player() {
}

void Player::handleInput(const SDL_Event& event, Level* level) {
    // Empty for now - using continuous input
}

void Player::checkContinuousInput(Level* level) {
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    
    // Reset pending movement
    pendingMoveX = 0;
    pendingMoveY = 0;
    
    // Check which keys are currently pressed
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
    
    // Try to move if not currently moving and there's pending movement
    if (!moving && (pendingMoveX != 0 || pendingMoveY != 0)) {
        move(pendingMoveX, pendingMoveY, level);
    }
}

void Player::startAnimation(const std::vector<int>& frames, float frameDuration) {
    currentAnimationFrames = frames;
    this->frameDuration = frameDuration;
    currentFrame = 0;
    animationTimer = 0.0f;
    isAnimating = true;
    
    if (!frames.empty()) {
        sprite.setSpriteId(frames[0]);
    }
}

void Player::updateAnimation(float deltaTime) {
    if (!isAnimating || currentAnimationFrames.empty()) {
        return;
    }
    
    animationTimer += deltaTime;
    
    if (animationTimer >= frameDuration) {
        animationTimer = 0.0f;
        currentFrame++;
        
        if (currentFrame >= static_cast<int>(currentAnimationFrames.size())) {
            currentFrame = 0;
            
            const Uint8* keystate = SDL_GetKeyboardState(NULL);
            bool anyMovementKey = keystate[SDL_SCANCODE_LEFT] || keystate[SDL_SCANCODE_A] ||
                                 keystate[SDL_SCANCODE_RIGHT] || keystate[SDL_SCANCODE_D] ||
                                 keystate[SDL_SCANCODE_UP] || keystate[SDL_SCANCODE_W] ||
                                 keystate[SDL_SCANCODE_DOWN] || keystate[SDL_SCANCODE_S];
            
            if (anyMovementKey) {
                sprite.setSpriteId(currentAnimationFrames[currentFrame]);
            } else {
                isAnimating = false;
                sprite.setSpriteId(idleSprite);
            }
        } else {
            sprite.setSpriteId(currentAnimationFrames[currentFrame]);
        }
    }
}

void Player::updateMovement(float deltaTime) {
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
    } else {
        float normalizedDx = dx / distanceToTarget;
        float normalizedDy = dy / distanceToTarget;
        
        renderX += normalizedDx * moveDistance;
        renderY += normalizedDy * moveDistance;
    }
}

void Player::move(int dx, int dy, Level* level) {
    int newX = x + dx;
    int newY = y + dy;
    
    if (level->isWalkable(newX, newY)) {
        if (level->getTile(newX, newY) == TileType::INFOTRON) {
            level->setTile(newX, newY, TileType::EMPTY);
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
            } else if (facingDirection == FacingDirection::RIGHT) {
                startAnimation({MURPHY_RIGHT_1, MURPHY_RIGHT_2, MURPHY_RIGHT_3, MURPHY_RIGHT_2, MURPHY_RIGHT_1}, ANIMATION_SPEED);
            } else {
                startAnimation({MURPHY_RIGHT_1, MURPHY_RIGHT_2, MURPHY_RIGHT_3, MURPHY_RIGHT_2, MURPHY_RIGHT_1}, ANIMATION_SPEED);
                facingDirection = FacingDirection::RIGHT;
                idleSprite = MURPHY_RIGHT_1;
            }
        }
    }
}

void Player::update(float deltaTime, Level* level) {
    checkContinuousInput(level);
    updateAnimation(deltaTime);
    updateMovement(deltaTime);
}

void Player::render(SDL_Renderer* renderer) {
    int pixelX = static_cast<int>(renderX * TILE_SIZE);
    int pixelY = static_cast<int>(renderY * TILE_SIZE);
    sprite.render(renderer, pixelX, pixelY);
}

void Player::renderAt(SDL_Renderer* renderer, int screenX, int screenY) {
    sprite.render(renderer, screenX, screenY);
}

// Add getter methods for smooth position
float Player::getRenderX() const {
    return renderX;
}

float Player::getRenderY() const {
    return renderY;
}