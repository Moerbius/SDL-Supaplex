#include "Game.hpp"
#include "Level.hpp"
#include "../entities/Player.hpp"
#include "../systems/AssetManager.hpp"
#include <chrono>
#include <algorithm>

const char* Game::WINDOW_TITLE = "SDL Supaplex";

Game::Game() : window(nullptr), sdlRenderer(nullptr), currentState(GameState::MENU), 
               isRunning(false), cameraX(0), cameraY(0) {
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
    
    // Initialize game objects
    currentLevel = std::make_unique<Level>();
    currentLevel->loadTestLevel();
    
    player = std::make_unique<Player>(5, 10); // Starting position
    
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
                } else if (currentState == GameState::PLAYING && player) {
                    player->handleInput(event, currentLevel.get());
                }
                break;
        }
    }
}

void Game::update(float deltaTime) {
    if (currentState == GameState::PLAYING) {
        if (player && currentLevel) {
            player->update(deltaTime, currentLevel.get());
            updateCamera(deltaTime);  // Pass deltaTime to updateCamera
        }
    }
}

void Game::updateCamera(float deltaTime) {  // Add deltaTime parameter
    if (!player) return;
    
    // Use smooth player position for camera
    float playerPixelX = player->getRenderX() * 16;
    float playerPixelY = player->getRenderY() * 16;
    
    // Center camera on player
    float targetCameraX = playerPixelX - (VIEWPORT_WIDTH / 2);
    float targetCameraY = playerPixelY - (VIEWPORT_HEIGHT / 2);
    
    // Clamp camera to level boundaries
    float maxCameraX = (Level::LEVEL_WIDTH * 16) - VIEWPORT_WIDTH;
    float maxCameraY = (Level::LEVEL_HEIGHT * 16) - VIEWPORT_HEIGHT;
    
    targetCameraX = std::max(0.0f, std::min(targetCameraX, maxCameraX));
    targetCameraY = std::max(0.0f, std::min(targetCameraY, maxCameraY));
    
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
        // Render frame first (background)
        renderFrame();
        
        // Set viewport for level area (inside the frame)
        SDL_Rect levelViewport = {VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT};
        SDL_RenderSetViewport(sdlRenderer, &levelViewport);
        SDL_RenderSetClipRect(sdlRenderer, &levelViewport);
        
        // Render level with camera offset
        renderLevelWithOffset();
        
        // Reset viewport and clip
        SDL_RenderSetViewport(sdlRenderer, nullptr);
        SDL_RenderSetClipRect(sdlRenderer, nullptr);
        
        // Render panel
        renderPanel();
    }
    
    // Present the back buffer
    SDL_RenderPresent(sdlRenderer);
}

void Game::renderLevelWithOffset() {
    // Calculate which tiles are visible
    int startTileX = static_cast<int>(cameraX / 16);
    int startTileY = static_cast<int>(cameraY / 16);
    int endTileX = startTileX + (VIEWPORT_WIDTH / 16) + 2;
    int endTileY = startTileY + (VIEWPORT_HEIGHT / 16) + 2;
    
    // Clamp to level bounds
    startTileX = std::max(0, startTileX);
    startTileY = std::max(0, startTileY);
    endTileX = std::min(Level::LEVEL_WIDTH, endTileX);
    endTileY = std::min(Level::LEVEL_HEIGHT, endTileY);
    
    // Render visible tiles with camera offset
    if (currentLevel) {
        currentLevel->renderRegion(sdlRenderer, startTileX, startTileY, endTileX, endTileY, -cameraX, -cameraY);
    }
    
    // Render player with camera offset using smooth position
    if (player) {
        float playerPixelX = player->getRenderX() * 16;
        float playerPixelY = player->getRenderY() * 16;
        int playerScreenX = static_cast<int>(playerPixelX - cameraX);
        int playerScreenY = static_cast<int>(playerPixelY - cameraY);
        player->renderAt(sdlRenderer, playerScreenX, playerScreenY);
    }
}

void Game::renderFrame() {
    SDL_Texture* frameTexture = AssetManager::getInstance().getTexture("frame");
    if (frameTexture) {
        // Render frame covering the entire screen
        SDL_Rect frameRect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT - PANEL_HEIGHT};
        SDL_RenderCopy(sdlRenderer, frameTexture, nullptr, &frameRect);
    }
}

void Game::renderPanel() {
    SDL_Texture* panelTexture = AssetManager::getInstance().getTexture("panel");
    if (panelTexture) {
        // Render panel at the bottom of the screen
        SDL_Rect panelRect = {0, WINDOW_HEIGHT - PANEL_HEIGHT, WINDOW_WIDTH, PANEL_HEIGHT};
        SDL_RenderCopy(sdlRenderer, panelTexture, nullptr, &panelRect);
    }
}

void Game::cleanup() {
    // Unique pointers will automatically clean up
    player.reset();
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