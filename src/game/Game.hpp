#ifndef GAME_HPP
#define GAME_HPP

#include "../main.hpp"
#include <memory>

// Forward declarations
class Level;
class Player;

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
    void renderFrame();
    void renderLevelWithOffset();
    void updateCamera(float deltaTime);
    
    SDL_Window* window;
    SDL_Renderer* sdlRenderer;
    
    GameState currentState;
    bool isRunning;
    
    // Game objects
    std::unique_ptr<Level> currentLevel;
    std::unique_ptr<Player> player;
    
    // Camera/viewport
    float cameraX, cameraY;
    
    // Constants matching original Supaplex
    static const int WINDOW_WIDTH = 320;
    static const int WINDOW_HEIGHT = 200;
    static const int SCALE_FACTOR = 3;  // Scale up for modern displays
    
    // Frame and viewport dimensions
    static const int FRAME_THICKNESS = 8;  // Adjust based on frame.png
    static const int VIEWPORT_WIDTH = 304;  // 320 - (8*2) frame thickness
    static const int VIEWPORT_HEIGHT = 184; // Adjust based on panel height
    static const int VIEWPORT_X = 8;        // Frame left edge
    static const int VIEWPORT_Y = 8;        // Frame top edge
    
    static const int PANEL_HEIGHT = 32;     // Adjust based on panel.png
    static const char* WINDOW_TITLE;
};

#endif // GAME_HPP