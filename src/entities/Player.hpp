#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "../main.hpp"
#include "../systems/Sprite.hpp"
#include <vector>

class Level;

enum class FacingDirection {
    LEFT,
    RIGHT,
    IDLE
};

class Player {
public:
    Player(int startX, int startY);
    ~Player();
    
    void handleInput(const SDL_Event& event, Level* level);
    void update(float deltaTime, Level* level);
    void render(SDL_Renderer* renderer);
    void renderAt(SDL_Renderer* renderer, int screenX, int screenY);
    
    int getX() const { return x; }
    int getY() const { return y; }
    float getRenderX() const;
    float getRenderY() const;
    bool isMoving() const { return moving; }
    
private:
    void move(int dx, int dy, Level* level);
    void startAnimation(const std::vector<int>& frames, float frameDuration);
    void updateAnimation(float deltaTime);
    void updateMovement(float deltaTime);
    void checkContinuousInput(Level* level);
    
    // Grid position
    int x, y;
    
    // Smooth movement
    float renderX, renderY;
    float targetX, targetY;
    bool moving;
    float moveSpeed;
    
    Sprite sprite;
    
    // Animation state
    std::vector<int> currentAnimationFrames;
    int currentFrame;
    float animationTimer;
    float frameDuration;
    bool isAnimating;
    int idleSprite;
    
    // Input state
    int pendingMoveX, pendingMoveY;
    FacingDirection facingDirection;
    
    // Murphy sprite IDs
    static const int MURPHY_IDLE = 3;
    static const int MURPHY_LEFT_1 = 8;
    static const int MURPHY_LEFT_2 = 9;
    static const int MURPHY_LEFT_3 = 10;
    static const int MURPHY_RIGHT_1 = 11;
    static const int MURPHY_RIGHT_2 = 12;
    static const int MURPHY_RIGHT_3 = 13;
    static const int MURPHY_UP = 3;
    static const int MURPHY_DOWN = 4;
    
    static const int TILE_SIZE = 16;
    static constexpr float ANIMATION_SPEED = 0.15f;
    static constexpr float MOVE_SPEED = 4.0f;
};

#endif // PLAYER_HPP