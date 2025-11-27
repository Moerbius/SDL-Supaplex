#ifndef BORDERSPRITE_HPP
#define BORDERSPRITE_HPP

#include "../main.hpp"

class BorderSprite {
public:
    BorderSprite();
    BorderSprite(SDL_Texture* texture, int spriteId);
    
    void setSpriteId(int spriteId);
    void render(SDL_Renderer* renderer, int x, int y, int quarter);
    void renderWithSprite(SDL_Renderer* renderer, int x, int y, int spriteId, int quarter);
    
private:
    SDL_Texture* texture;
    SDL_Rect srcRect;
    
    static const int SPRITE_SIZE = 16;
    static const int QUARTER_SIZE = 8;
    static const int SPRITES_PER_ROW = 16;
    
    void setQuarterSrcRect(int spriteId, int quarter);
};

#endif // BORDERSPRITE_HPP
