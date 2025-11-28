#ifndef GAME_HPP
#define GAME_HPP

#include "../main.hpp"
#include <memory>

// Forward declarations
class Level;
//class Player;

class Game {
public:
    Game();
    ~Game();
    
    bool initialize();
    void run();
    void cleanup();
    
private:
    void handleEvents();
    void update(float deltaTime);
    void render();
    void renderPanel();
    void renderLevelWithOffset();
    void updateCamera(float deltaTime);
    
    SDL_Window* window;
    SDL_Renderer* sdlRenderer;
    
    GameState currentState;
    bool isRunning;
    
    // Game objects
    std::unique_ptr<Level> currentLevel;
    // Remove: std::unique_ptr<Player> player;
    
    // Camera/viewport
    float cameraX, cameraY;
    
    // Constants matching original Supaplex
    static const int WINDOW_WIDTH = 320;
    static const int WINDOW_HEIGHT = 200;
    static const int SCALE_FACTOR = 2;  // Scale up for modern displays
    
    // Dynamic viewport dimensions
    int viewportWidth;
    int viewportHeight;
    int panelHeight;
    
    static const char* WINDOW_TITLE;
};

#endif // GAME_HPP