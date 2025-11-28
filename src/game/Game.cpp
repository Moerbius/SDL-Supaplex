#include "Game.hpp"
#include "Level.hpp"
#include "../systems/AssetManager.hpp"
#include <chrono>
#include <algorithm>

const char* Game::WINDOW_TITLE = "SDL Supaplex";

Game::Game() : window(nullptr), sdlRenderer(nullptr), currentState(GameState::MENU), 
               isRunning(false), cameraX(0), cameraY(0), 
               viewportWidth(0), viewportHeight(0), panelHeight(0) {
}

Game::~Game() {
    cleanup();
}

bool Game::initialize() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Create window with original resolution scaled up
    window = SDL_CreateWindow(WINDOW_TITLE,
                             SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED,
                             WINDOW_WIDTH * SCALE_FACTOR, 
                             WINDOW_HEIGHT * SCALE_FACTOR,
                             SDL_WINDOW_SHOWN);
    
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Create renderer
    sdlRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!sdlRenderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Set logical size to original resolution
    SDL_RenderSetLogicalSize(sdlRenderer, WINDOW_WIDTH, WINDOW_HEIGHT);
    
    // Set renderer color
    SDL_SetRenderDrawColor(sdlRenderer, 0x00, 0x00, 0x00, 0xFF);
    
    // Initialize AssetManager
    if (!AssetManager::getInstance().initialize(sdlRenderer)) {
        std::cerr << "Failed to initialize AssetManager!" << std::endl;
        return false;
    }
    
    // Get panel height from loaded texture
    panelHeight = AssetManager::getInstance().getTextureHeight("panel");
    if (panelHeight == 0) {
        panelHeight = 32; // Fallback value
        std::cerr << "Warning: Could not get panel height, using default 32px" << std::endl;
    }
    
    // Calculate viewport dimensions to use full window area above panel
    viewportWidth = WINDOW_WIDTH;
    viewportHeight = WINDOW_HEIGHT - panelHeight;
    
    // Initialize game objects
    currentLevel = std::make_unique<Level>();
    currentLevel->loadTestLevel();
    
    std::cout << "Game initialized successfully!" << std::endl;
    return true;
}

void Game::run() {
    if (!initialize()) {
        return;
    }
    
    isRunning = true;
    currentState = GameState::PLAYING;
    
    auto lastTime = std::chrono::high_resolution_clock::now();
    
    while (isRunning) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;
        
        handleEvents();
        update(deltaTime);
        render();
    }
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                isRunning = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    isRunning = false;
                } else if (currentState == GameState::PLAYING && currentLevel) {
                    MurphyObject* murphy = currentLevel->getMurphy();
                    if (murphy) {
                        murphy->handleInput(event, currentLevel.get());
                    }
                }
                break;
        }
    }
}

void Game::update(float deltaTime) {
    if (currentState == GameState::PLAYING) {
        if (currentLevel) {
            currentLevel->update(deltaTime);
            updateCamera(deltaTime);
        }
    }
}

void Game::updateCamera(float deltaTime) {
    if (!currentLevel) return;
    
    MurphyObject* murphy = currentLevel->getMurphy();
    if (!murphy) return;
    
    // Use Murphy's position for camera
    float playerPixelX = murphy->getRenderX() * 16;
    float playerPixelY = murphy->getRenderY() * 16;
    
    // Center camera on player using dynamic viewport
    float targetCameraX = playerPixelX - (viewportWidth / 2);
    float targetCameraY = playerPixelY - (viewportHeight / 2);
    
    // Expand camera bounds to include border areas (allow negative camera positions)
    float maxCameraX = (Level::LEVEL_WIDTH * 16) - viewportWidth + 8;  // Extra space for borders
    float maxCameraY = (Level::LEVEL_HEIGHT * 16) - viewportHeight + 8; // Extra space for borders
    
    // Allow camera to go slightly negative to show borders
    targetCameraX = std::max(-8.0f, std::min(targetCameraX, maxCameraX)); // Allow -32 to show left border
    targetCameraY = std::max(-8.0f, std::min(targetCameraY, maxCameraY)); // Allow -32 to show top border
    
    // Smooth camera following
    float cameraSpeed = 8.0f;
    cameraX += (targetCameraX - cameraX) * cameraSpeed * deltaTime;
    cameraY += (targetCameraY - cameraY) * cameraSpeed * deltaTime;
}

void Game::render() {
    // Clear screen with dark background
    SDL_SetRenderDrawColor(sdlRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(sdlRenderer);
    
    if (currentState == GameState::PLAYING) {
        // Set viewport for level area using full window width and height above panel
        SDL_Rect levelViewport = {0, 0, viewportWidth, viewportHeight};
        SDL_RenderSetViewport(sdlRenderer, &levelViewport);
        SDL_RenderSetClipRect(sdlRenderer, &levelViewport);
        
        // Render level content within the clipped viewport
        renderLevelWithOffset();
        
        // Reset viewport and clip for UI elements
        SDL_RenderSetViewport(sdlRenderer, nullptr);
        SDL_RenderSetClipRect(sdlRenderer, nullptr);
        
        // Render panel (UI layer on top)
        renderPanel();
    }
    
    // Present the back buffer
    SDL_RenderPresent(sdlRenderer);
}

void Game::renderLevelWithOffset() {
    // Calculate which tiles are visible - expand to include borders
    int startTileX = static_cast<int>(cameraX / 16) - 2;  // Extra margin for borders
    int startTileY = static_cast<int>(cameraY / 16) - 2;  // Extra margin for borders
    int endTileX = startTileX + (viewportWidth / 16) + 4;   // More tiles for borders
    int endTileY = startTileY + (viewportHeight / 16) + 4;  // More tiles for borders
    
    // Don't clamp to level bounds - allow rendering outside level for borders
    // startTileX = std::max(0, startTileX);
    // startTileY = std::max(0, startTileY);
    // endTileX = std::min(Level::LEVEL_WIDTH, endTileX);
    // endTileY = std::min(Level::LEVEL_HEIGHT, endTileY);
    
    // Render visible tiles and borders with camera offset
    if (currentLevel) {
        currentLevel->renderRegion(sdlRenderer, startTileX, startTileY, endTileX, endTileY, -cameraX, -cameraY);
    }
}

void Game::renderPanel() {
    SDL_Texture* panelTexture = AssetManager::getInstance().getTexture("panel");
    if (panelTexture) {
        // Render panel at the bottom using dynamic height
        SDL_Rect panelRect = {0, WINDOW_HEIGHT - panelHeight, WINDOW_WIDTH, panelHeight};
        SDL_RenderCopy(sdlRenderer, panelTexture, nullptr, &panelRect);
    }
}

void Game::cleanup() {
    // Unique pointers will automatically clean up
    currentLevel.reset();
    
    // Clean up AssetManager
    AssetManager::getInstance().cleanup();
    
    if (sdlRenderer) {
        SDL_DestroyRenderer(sdlRenderer);
        sdlRenderer = nullptr;
    }
    
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    
    SDL_Quit();
}