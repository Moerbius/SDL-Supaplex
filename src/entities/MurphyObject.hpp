#ifndef MURPHYOBJECT_HPP
#define MURPHYOBJECT_HPP

#include "GameObject.hpp"
#include <vector>

class Level;

enum class FacingDirection {
    LEFT,
    RIGHT,
    IDLE
};

class MurphyObject : public GameObject {
public:
    MurphyObject(int startX, int startY);
    
    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer, float offsetX, float offsetY) override;
    
    void handleInput(const SDL_Event& event, Level* level);
    void processInput(Level* level);
    
    float getRenderX() const { return renderX; }
    float getRenderY() const { return renderY; }
    bool isMoving() const { return moving; }
    
private:
    void move(int dx, int dy, Level* level);
    void dig(int dx, int dy, Level* level);
    void startAnimation(const std::vector<int>& frames, float frameDuration);
    void updateAnimation(float deltaTime);
    void updateMovement(float deltaTime);
    void checkContinuousInput(Level* level);
    
    float renderX, renderY;
    float targetX, targetY;
    bool moving;
    float moveSpeed;
    
    std::vector<int> currentAnimationFrames;
    int currentFrame;
    float animationTimer;
    float frameDuration;
    bool isAnimating;
    int idleSprite;
    
    int pendingMoveX, pendingMoveY;
    FacingDirection facingDirection;
    bool isDigging;
    
    bool hasPendingObjectRemoval;
    int pendingRemovalX, pendingRemovalY;
    Level* pendingLevel;
    
    // Add movement tracking for gravity triggers
    int previousX, previousY;  // Track where Murphy was before moving
    
    static const int MURPHY_IDLE = 3;
    static const int MURPHY_LEFT_1 = 8;
    static const int MURPHY_LEFT_2 = 9;
    static const int MURPHY_LEFT_3 = 10;
    static const int MURPHY_RIGHT_1 = 11;
    static const int MURPHY_RIGHT_2 = 12;
    static const int MURPHY_RIGHT_3 = 13;
    
    // Digging sprites for each direction
    static const int MURPHY_DIG_UP = 14;
    static const int MURPHY_DIG_DOWN = 15;
    static const int MURPHY_DIG_LEFT = 25;
    static const int MURPHY_DIG_RIGHT = 24;
    
    static constexpr float ANIMATION_SPEED = 0.15f;
    static constexpr float MOVE_SPEED = 8.0f;  // Changed from 4.0f to 8.0f to match zonk fall speed
};

#endif // MURPHYOBJECT_HPP
