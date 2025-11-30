#include "ZonkObject.hpp"
#include "../game/Level.hpp"

// Define the rolling animation sprites
const std::vector<int> ZonkObject::ROLL_RIGHT_SPRITES = {97, 98, 99};
const std::vector<int> ZonkObject::ROLL_LEFT_SPRITES = {99, 98, 97};

ZonkObject::ZonkObject(int x, int y) 
    : GameObject(x, y, ObjectType::ZONK), falling(false), rolling(false), 
      fallTimer(0.0f), renderY(static_cast<float>(y)), renderX(static_cast<float>(x)),
      rollDirection(0), rollSpeed(ROLL_SPEED), rollAnimationTimer(0.0f), 
      rollAnimationFrame(0), currentLevel(nullptr) {
    setSpriteId(SPRITE_ZONK);
}

void ZonkObject::update(float deltaTime) {
    // Update fall timer for gravity checks
    fallTimer += deltaTime;
    
    // Check for gravity periodically when not falling or rolling
    float checkInterval = (falling || rolling) ? GRAVITY_CHECK_INTERVAL * 2 : GRAVITY_CHECK_INTERVAL;
    
    if (fallTimer >= checkInterval) {
        fallTimer = 0.0f;
        if (currentLevel && !rolling) {
            checkGravity(currentLevel);
            
            // Also check if zonk should roll off objects it's resting on
            if (!falling) {
                checkStaticRolling(currentLevel);
            }
        }
    }
    
    // Update falling animation
    if (falling) {
        updateFalling(deltaTime);
    }
    
    // Update rolling animation
    if (rolling) {
        updateRolling(deltaTime);
        updateRollingAnimation(deltaTime);
    }
}

void ZonkObject::render(SDL_Renderer* renderer, float offsetX, float offsetY) {
    if (!active) return;
    
    // Use smooth renderX and renderY for falling/rolling animation
    int pixelX = static_cast<int>(roundf((renderX * TILE_SIZE) + offsetX));
    int pixelY = static_cast<int>(roundf((renderY * TILE_SIZE) + offsetY));
    sprite.render(renderer, pixelX, pixelY);
}

void ZonkObject::checkGravity(Level* level) {
    if (!level || falling) return;
    
    int belowY = y + 1;
    
    // Check if there's empty space directly below
    if (belowY < Level::LEVEL_HEIGHT) {
        GameObject* objBelow = level->getObjectAt(x, belowY);
        
        if (!objBelow) {
            // Empty space below - start falling to that tile
            falling = true;
            y = belowY;
            renderY = static_cast<float>(y - 1);
        } else {
            // Hit an obstacle - check if we should roll off
            if (objBelow->getType() == ObjectType::ZONK || objBelow->getType() == ObjectType::INFOTRON) {
                tryRollOff(level, belowY);
            }
        }
    }
}

void ZonkObject::checkStaticRolling(Level* level) {
    if (!level || falling || rolling) return;
    
    // Check if we're resting on a zonk or infotron
    int belowY = y + 1;
    if (belowY >= Level::LEVEL_HEIGHT) return;
    
    GameObject* objBelow = level->getObjectAt(x, belowY);
    if (!objBelow) return;
    
    // Check if we're resting on a zonk or infotron
    if (objBelow->getType() == ObjectType::ZONK || objBelow->getType() == ObjectType::INFOTRON) {
        // Try to roll off - check right side first, then left
        int directions[] = {1, -1};
        
        for (int dir : directions) {
            int sideX = x + dir;
            
            // Check bounds
            if (sideX < 0 || sideX >= Level::LEVEL_WIDTH) continue;
            
            // Check if side position is empty
            GameObject* sideObj = level->getObjectAt(sideX, y);
            if (sideObj) continue;
            
            // Check if there's space below the side position
            GameObject* belowSideObj = level->getObjectAt(sideX, belowY);
            if (belowSideObj) continue;
            
            // Start rolling in this direction with animation
            rolling = true;
            rollDirection = dir;
            renderX = static_cast<float>(x);
            
            // Set up animation frames based on direction
            if (dir > 0) {
                rollAnimationFrames = ROLL_RIGHT_SPRITES;
            } else {
                rollAnimationFrames = ROLL_LEFT_SPRITES;
            }
            
            rollAnimationFrame = 0;
            rollAnimationTimer = 0.0f;
            setSpriteId(rollAnimationFrames[0]);
            
            return;
        }
    }
}

void ZonkObject::updateFalling(float deltaTime) {
    if (!falling) return;
    
    // Animate smooth movement to the new grid position
    float targetY = static_cast<float>(y);
    renderY += FALL_SPEED * deltaTime;
    
    // Check if we've reached the target position
    if (renderY >= targetY) {
        renderY = targetY;
        falling = false;
    }
}

void ZonkObject::updateRollingAnimation(float deltaTime) {
    if (!rolling || rollAnimationFrames.empty()) return;
    
    rollAnimationTimer += deltaTime;
    
    if (rollAnimationTimer >= ROLL_ANIMATION_SPEED) {
        rollAnimationTimer = 0.0f;
        rollAnimationFrame = (rollAnimationFrame + 1) % rollAnimationFrames.size();
        setSpriteId(rollAnimationFrames[rollAnimationFrame]);
    }
}

void ZonkObject::updateRolling(float deltaTime) {
    if (!rolling) return;
    
    // Move horizontally at the slower speed
    renderX += rollDirection * rollSpeed * deltaTime;
    
    // Check if we've reached the target position
    float targetX = static_cast<float>(x + rollDirection);
    
    if ((rollDirection > 0 && renderX >= targetX) || 
        (rollDirection < 0 && renderX <= targetX)) {
        
        // Reached target position
        x = x + rollDirection;
        renderX = static_cast<float>(x);
        rolling = false;
        
        // Reset to normal zonk sprite
        setSpriteId(SPRITE_ZONK);
        rollAnimationTimer = 0.0f;
        rollAnimationFrame = 0;
        rollAnimationFrames.clear();
        
        // Start falling from new position
        if (currentLevel) {
            checkGravity(currentLevel);
        }
    }
}

void ZonkObject::tryRollOff(Level* level, int obstacleY) {
    if (!level || rolling) return;
    
    // Try right side first, then left
    int directions[] = {1, -1};
    
    for (int dir : directions) {
        int newX = x + dir;
        
        if (canRollTo(level, newX, obstacleY)) {
            // Start rolling in this direction with animation
            rolling = true;
            falling = false;
            rollDirection = dir;
            renderX = static_cast<float>(x);
            
            // Set up animation frames based on direction
            if (dir > 0) {
                rollAnimationFrames = ROLL_RIGHT_SPRITES;
            } else {
                rollAnimationFrames = ROLL_LEFT_SPRITES;
            }
            
            rollAnimationFrame = 0;
            rollAnimationTimer = 0.0f;
            setSpriteId(rollAnimationFrames[0]);
            
            return;
        }
    }
    
    // Can't roll - just stop falling
    falling = false;
    renderY = static_cast<float>(y);
}

bool ZonkObject::canRollTo(Level* level, int newX, int belowY) {
    if (!level) return false;
    
    // Check bounds
    if (newX < 0 || newX >= Level::LEVEL_WIDTH) return false;
    
    // Check if the side position is empty
    GameObject* sideObj = level->getObjectAt(newX, y);
    if (sideObj) return false;
    
    // Check if there's space below the side position for continued falling
    if (belowY < Level::LEVEL_HEIGHT) {
        GameObject* belowSideObj = level->getObjectAt(newX, belowY);
        if (!belowSideObj) {
            // There's space to continue falling after rolling
            return true;
        }
    }
    
    return false;
}
