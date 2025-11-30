#include "GameObject.hpp"
#include "../systems/AssetManager.hpp"

GameObject::GameObject(int x, int y, ObjectType type) 
    : x(x), y(y), type(type), active(true) {
    
    SDL_Texture* spriteTexture = AssetManager::getInstance().getTexture("sprites");
    sprite = Sprite(spriteTexture, 0); // Default sprite
}

void GameObject::render(SDL_Renderer* renderer, float offsetX, float offsetY) {
    if (!active) return;
    
    // Ensure pixel-perfect positioning by rounding to nearest pixel
    int renderX = static_cast<int>(roundf((x * TILE_SIZE) + offsetX));
    int renderY = static_cast<int>(roundf((y * TILE_SIZE) + offsetY));
    sprite.render(renderer, renderX, renderY);
}

void GameObject::setSpriteId(int spriteId) {
    sprite.setSpriteId(spriteId);
}
