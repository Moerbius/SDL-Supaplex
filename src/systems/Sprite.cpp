#include "Sprite.hpp"

Sprite::Sprite() : texture(nullptr) {
    srcRect = {0, 0, SPRITE_SIZE, SPRITE_SIZE};
}

Sprite::Sprite(SDL_Texture* texture, int spriteId) : texture(texture) {
    setSpriteId(spriteId);
}

void Sprite::setSpriteId(int spriteId) {
    int col = spriteId % SPRITES_PER_ROW;
    int row = spriteId / SPRITES_PER_ROW;
    
    srcRect.x = col * SPRITE_SIZE;
    srcRect.y = row * SPRITE_SIZE;
    srcRect.w = SPRITE_SIZE;
    srcRect.h = SPRITE_SIZE;
}

void Sprite::render(SDL_Renderer* renderer, int x, int y) {
    // Render at 16x16 size to match the game grid
    SDL_Rect dstRect = {x, y, 16, 16};
    SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);
}

void Sprite::render(SDL_Renderer* renderer, int x, int y, int width, int height) {
    SDL_Rect dstRect = {x, y, width, height};
    SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);
}