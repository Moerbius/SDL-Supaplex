#include "Level.hpp"
#include "../systems/AssetManager.hpp"

// Define static constants
const int Level::LEVEL_WIDTH;
const int Level::LEVEL_HEIGHT;
const int Level::TILE_SIZE;
const int Level::SPRITE_EMPTY;
const int Level::SPRITE_WALL;
const int Level::SPRITE_INFOTRON;
const int Level::SPRITE_ZONK;

Level::Level() {
    // Initialize all tiles as empty
    for (int y = 0; y < LEVEL_HEIGHT; y++) {
        for (int x = 0; x < LEVEL_WIDTH; x++) {
            tiles[y][x] = TileType::EMPTY;
        }
    }
    
    // Initialize tile sprite
    SDL_Texture* spriteTexture = AssetManager::getInstance().getTexture("sprites");
    if (spriteTexture) {
        tileSprite = Sprite(spriteTexture, SPRITE_EMPTY);
    }
}

Level::~Level() {
}

void Level::loadTestLevel() {
    // Create border walls
    for (int x = 0; x < LEVEL_WIDTH; x++) {
        tiles[0][x] = TileType::WALL;
        tiles[LEVEL_HEIGHT - 1][x] = TileType::WALL;
    }
    for (int y = 0; y < LEVEL_HEIGHT; y++) {
        tiles[y][0] = TileType::WALL;
        tiles[y][LEVEL_WIDTH - 1] = TileType::WALL;
    }
    
    // Add some random walls and infotrons
    tiles[5][10] = TileType::WALL;
    tiles[5][11] = TileType::WALL;
    tiles[5][12] = TileType::WALL;
    
    tiles[7][15] = TileType::INFOTRON;
    tiles[10][20] = TileType::INFOTRON;
    tiles[8][25] = TileType::INFOTRON;
    
    // Add some zonks
    tiles[6][30] = TileType::ZONK;
    tiles[8][35] = TileType::ZONK;
}

void Level::render(SDL_Renderer* renderer) {
    for (int y = 0; y < LEVEL_HEIGHT; y++) {
        for (int x = 0; x < LEVEL_WIDTH; x++) {
            TileType tile = tiles[y][x];
            
            // Skip empty tiles and player tile (player renders itself)
            if (tile == TileType::EMPTY || tile == TileType::PLAYER) {
                continue;
            }
            
            // Set appropriate sprite ID based on tile type
            switch (tile) {
                case TileType::WALL:
                    tileSprite.setSpriteId(SPRITE_WALL);
                    break;
                case TileType::INFOTRON:
                    tileSprite.setSpriteId(SPRITE_INFOTRON);
                    break;
                case TileType::ZONK:
                    tileSprite.setSpriteId(SPRITE_ZONK);
                    break;
                default:
                    continue;
            }
            
            tileSprite.render(renderer, x * TILE_SIZE, y * TILE_SIZE);
        }
    }
}

void Level::renderRegion(SDL_Renderer* renderer, int startX, int startY, int endX, int endY, float offsetX, float offsetY) {
    for (int y = startY; y < endY; y++) {
        for (int x = startX; x < endX; x++) {
            if (x < 0 || x >= LEVEL_WIDTH || y < 0 || y >= LEVEL_HEIGHT) {
                continue;
            }
            
            TileType tile = tiles[y][x];
            
            // Skip empty tiles and player tile (player renders itself)
            if (tile == TileType::EMPTY || tile == TileType::PLAYER) {
                continue;
            }
            
            // Set appropriate sprite ID based on tile type
            switch (tile) {
                case TileType::WALL:
                    tileSprite.setSpriteId(SPRITE_WALL);
                    break;
                case TileType::INFOTRON:
                    tileSprite.setSpriteId(SPRITE_INFOTRON);
                    break;
                case TileType::ZONK:
                    tileSprite.setSpriteId(SPRITE_ZONK);
                    break;
                default:
                    continue;
            }
            
            int renderX = static_cast<int>((x * TILE_SIZE) + offsetX);
            int renderY = static_cast<int>((y * TILE_SIZE) + offsetY);
            tileSprite.render(renderer, renderX, renderY);
        }
    }
}

bool Level::isWalkable(int x, int y) const {
    if (x < 0 || x >= LEVEL_WIDTH || y < 0 || y >= LEVEL_HEIGHT) {
        return false;
    }
    
    TileType tile = tiles[y][x];
    return tile == TileType::EMPTY || tile == TileType::INFOTRON;
}

TileType Level::getTile(int x, int y) const {
    if (x < 0 || x >= LEVEL_WIDTH || y < 0 || y >= LEVEL_HEIGHT) {
        return TileType::WALL;
    }
    return tiles[y][x];
}

void Level::setTile(int x, int y, TileType type) {
    if (x >= 0 && x < LEVEL_WIDTH && y >= 0 && y < LEVEL_HEIGHT) {
        tiles[y][x] = type;
    }
}