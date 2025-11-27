#include "AssetManager.hpp"
#include <SDL2/SDL_image.h>

AssetManager& AssetManager::getInstance() {
    static AssetManager instance;
    return instance;
}

bool AssetManager::initialize(SDL_Renderer* renderer) {
    this->renderer = renderer;
    
    // Initialize SDL_image
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }
    
    // Load the main sprite sheet
    if (!loadTexture("sprites", "assets/gfx/RocksSP.png")) {
        std::cerr << "Failed to load sprite sheet!" << std::endl;
        return false;
    }
    
    // Load the panel
    if (!loadTexture("panel", "assets/gfx/panel.png")) {
        std::cerr << "Failed to load panel!" << std::endl;
        return false;
    }
    
    return true;
}

void AssetManager::cleanup() {
    for (auto& pair : textures) {
        SDL_DestroyTexture(pair.second);
    }
    textures.clear();
    
    IMG_Quit();
}

SDL_Texture* AssetManager::getTexture(const std::string& name) {
    auto it = textures.find(name);
    if (it != textures.end()) {
        return it->second;
    }
    return nullptr;
}

bool AssetManager::loadTexture(const std::string& name, const std::string& path) {
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (!loadedSurface) {
        std::cerr << "Unable to load image " << path << "! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
    
    if (!texture) {
        std::cerr << "Unable to create texture from " << path << "! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    textures[name] = texture;
    return true;
}

int AssetManager::getTextureWidth(const std::string& name) {
    SDL_Texture* texture = getTexture(name);
    if (texture) {
        int width;
        SDL_QueryTexture(texture, nullptr, nullptr, &width, nullptr);
        return width;
    }
    return 0;
}

int AssetManager::getTextureHeight(const std::string& name) {
    SDL_Texture* texture = getTexture(name);
    if (texture) {
        int height;
        SDL_QueryTexture(texture, nullptr, nullptr, nullptr, &height);
        return height;
    }
    return 0;
}