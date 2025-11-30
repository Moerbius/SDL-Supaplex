#ifndef ZONKOBJECT_HPP
#define ZONKOBJECT_HPP

#include "GameObject.hpp"
#include <vector>

class Level;  // Forward declaration instead of include

class ZonkObject : public GameObject {
public:
    ZonkObject(int x, int y);
    
    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer, float offsetX, float offsetY) override;
    
    bool canBePushed() const { return !falling && !rolling; }
    bool isFalling() const { return falling; }
    bool isRolling() const { return rolling; }
    
    void setLevel(Level* level) { currentLevel = level; }
    void forceGravityCheck() { fallTimer = GRAVITY_CHECK_INTERVAL; }
    
private:
    void checkGravity(Level* level);
    void updateFalling(float deltaTime);
    void updateRolling(float deltaTime);
    void updateRollingAnimation(float deltaTime);  // Add rolling animation
    void tryRollOff(Level* level, int obstacleY);
    void checkStaticRolling(Level* level);
    bool canRollTo(Level* level, int newX, int belowY);
    
    bool falling;
    bool rolling;
    float fallTimer;
    float renderY;
    float renderX;
    
    int rollDirection;
    float rollSpeed;
    
    // Rolling animation
    float rollAnimationTimer;
    int rollAnimationFrame;
    std::vector<int> rollAnimationFrames;
    
    Level* currentLevel;
    
    static const int SPRITE_ZONK = 1;
    static constexpr float FALL_SPEED = 4.0f;
    static constexpr float ROLL_SPEED = 3.0f;
    static constexpr float GRAVITY_CHECK_INTERVAL = 0.05f;
    static constexpr float ROLL_ANIMATION_SPEED = 0.1f;  // Animation frame duration
    
    // Rolling animation sprites
    static const std::vector<int> ROLL_RIGHT_SPRITES;  // 97, 98, 99
    static const std::vector<int> ROLL_LEFT_SPRITES;   // 99, 98, 97
};

#endif // ZONKOBJECT_HPP
