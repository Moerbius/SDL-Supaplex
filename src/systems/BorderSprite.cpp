#include "BorderSprite.hpp"

BorderSprite::BorderSprite() : texture(nullptr) {
    srcRect = {0, 0, QUARTER_SIZE, QUARTER_SIZE};
}

BorderSprite::BorderSprite(SDL_Texture* texture, int spriteId) : texture(texture) {
    setSpriteId(spriteId);
}

void BorderSprite::setSpriteId(int spriteId) {
    int col = spriteId % SPRITES_PER_ROW;
    int row = spriteId / SPRITES_PER_ROW;
    
    srcRect.x = col * SPRITE_SIZE;
    srcRect.y = row * SPRITE_SIZE;
    srcRect.w = QUARTER_SIZE;
    srcRect.h = QUARTER_SIZE;
}

void BorderSprite::setQuarterSrcRect(int spriteId, int quarter) {
    int col = spriteId % SPRITES_PER_ROW;
    int row = spriteId / SPRITES_PER_ROW;
    int baseX = col * SPRITE_SIZE;
    int baseY = row * SPRITE_SIZE;
    
    switch (quarter) {
        case 0: // Bottom right
            srcRect.x = baseX + QUARTER_SIZE;
            srcRect.y = baseY + QUARTER_SIZE;
            break;
        case 1: // Bottom left
            srcRect.x = baseX;
            srcRect.y = baseY + QUARTER_SIZE;
            break;
        case 2: // Upper right
            srcRect.x = baseX + QUARTER_SIZE;
            srcRect.y = baseY;
            break;
        case 3: // Upper left
            srcRect.x = baseX;
            srcRect.y = baseY;
            break;
        default:
            srcRect.x = baseX;
            srcRect.y = baseY;
            break;
    }
    
    srcRect.w = QUARTER_SIZE;
    srcRect.h = QUARTER_SIZE;
}

void BorderSprite::renderWithSprite(SDL_Renderer* renderer, int x, int y, int spriteId, int quarter) {
    if (!texture) return;
    
    setQuarterSrcRect(spriteId, quarter);
    SDL_Rect dstRect = {x, y, 16, 16};
    SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);
}

void BorderSprite::render(SDL_Renderer* renderer, int x, int y, int quarter) {
    int baseSpriteId = srcRect.x / SPRITE_SIZE + (srcRect.y / SPRITE_SIZE) * SPRITES_PER_ROW;
    renderWithSprite(renderer, x, y, baseSpriteId, quarter);
}
