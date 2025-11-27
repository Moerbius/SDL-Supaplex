#ifndef ASSETMANAGER_HPP
#define ASSETMANAGER_HPP

#include "../main.hpp"
#include <unordered_map>
#include <string>

class AssetManager {
public:
    static AssetManager& getInstance();
    
    bool initialize(SDL_Renderer* renderer);
    void cleanup();
    
    SDL_Texture* getTexture(const std::string& name);
    bool loadTexture(const std::string& name, const std::string& path);
    
    // Get texture dimensions
    int getTextureWidth(const std::string& name);
    int getTextureHeight(const std::string& name);
    
    // Sprite sheet info for Supaplex
    static const int SPRITE_SIZE = 16;     // Fixed back to 16
    static const int SPRITES_PER_ROW = 16;
    
private:
    AssetManager() = default;
    ~AssetManager() = default;
    
    SDL_Renderer* renderer;
    std::unordered_map<std::string, SDL_Texture*> textures;
};

#endif // ASSETMANAGER_HPP