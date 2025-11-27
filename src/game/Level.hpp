#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "../main.hpp"
#include "../systems/Sprite.hpp"
#include "../systems/BorderSprite.hpp"
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
    static const int TILE_SIZE = 16;  // Fixed back to 16
    
    // Sprite IDs for different tiles (adjust based on your sprite sheet)
    static const int SPRITE_EMPTY = 0;
    static const int SPRITE_INFOTRON = 4;
    static const int SPRITE_TERMINAL = 35;  //TODO: check this
    static const int SPRITE_BASE = 2;       //TODO: check this
    static const int SPRITE_EXIT = 7;       //TODO: check this
    static const int SPRITE_BUG = 50;       //TODO: check this
    static const int SPRITE_SIK_SNAK = 51;  //TODO: check this
    
    static const int SPRITE_ZONK = 1;
    
    // Border sprite IDs
    static const int SPRITE_BORDER_CORNERS = 229; //245;     // Corner sprite (last row, 6th from left)
    static const int SPRITE_BORDER_VERTICAL = 230;    // Left/Right borders (last row, 7th from left)
    static const int SPRITE_BORDER_HORIZONTAL = 231;  // Top/Bottom borders (last row, 8th from left)

private:
    std::array<std::array<TileType, LEVEL_WIDTH>, LEVEL_HEIGHT> tiles;
    Sprite tileSprite;
    BorderSprite borderSprite;
    
    void renderBorders(SDL_Renderer* renderer, int startX, int startY, int endX, int endY, float offsetX, float offsetY);
};

#endif // LEVEL_HPP