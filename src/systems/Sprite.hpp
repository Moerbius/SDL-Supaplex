#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "../main.hpp"

class Sprite {
public:
    Sprite();
    Sprite(SDL_Texture* texture, int spriteId);
    
    void setSpriteId(int spriteId);
    void render(SDL_Renderer* renderer, int x, int y);
    void render(SDL_Renderer* renderer, int x, int y, int width, int height);
    
private:
    SDL_Texture* texture;
    SDL_Rect srcRect;
    
    static const int SPRITE_SIZE = 16;     // Fixed back to 16
    static const int SPRITES_PER_ROW = 16;
};

#endif // SPRITE_HPP