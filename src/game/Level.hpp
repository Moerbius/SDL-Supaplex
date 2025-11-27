#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "../main.hpp"
#include "../systems/Sprite.hpp"
#include <array>

class Level {
public:
    Level();
    ~Level();
    
    void loadTestLevel();
    void render(SDL_Renderer* renderer);
    void renderRegion(SDL_Renderer* renderer, int startX, int startY, int endX, int endY, float offsetX, float offsetY);
    bool isWalkable(int x, int y) const;
    TileType getTile(int x, int y) const;
    void setTile(int x, int y, TileType type);
    
    static const int LEVEL_WIDTH = 60;
    static const int LEVEL_HEIGHT = 24;
    static const int TILE_SIZE = 16;
    
    // Sprite IDs for different tiles (adjust based on your sprite sheet)
    static const int SPRITE_EMPTY = 0;
    static const int SPRITE_WALL = 1;
    static const int SPRITE_INFOTRON = 2;
    static const int SPRITE_ZONK = 3;
    
private:
    std::array<std::array<TileType, LEVEL_WIDTH>, LEVEL_HEIGHT> tiles;
    Sprite tileSprite;
};

#endif // LEVEL_HPP