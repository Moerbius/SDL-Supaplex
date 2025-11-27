#include "Level.hpp"
#include "../systems/AssetManager.hpp"

// Define static constants - match exactly what's in the header
const int Level::LEVEL_WIDTH;
const int Level::LEVEL_HEIGHT;
const int Level::TILE_SIZE;

const int Level::SPRITE_EMPTY;
const int Level::SPRITE_INFOTRON;
const int Level::SPRITE_ZONK;
const int Level::SPRITE_TERMINAL;
const int Level::SPRITE_BASE;
const int Level::SPRITE_EXIT;
const int Level::SPRITE_BUG;
const int Level::SPRITE_SIK_SNAK;

const int Level::SPRITE_BORDER_CORNERS;
const int Level::SPRITE_BORDER_VERTICAL;
const int Level::SPRITE_BORDER_HORIZONTAL;

Level::Level() {
    // Initialize all tiles as empty
    for (int y = 0; y < LEVEL_HEIGHT; y++) {
        for (int x = 0; x < LEVEL_WIDTH; x++) {
            tiles[y][x] = TileType::EMPTY;
        }
    }
    
    // Initialize tile sprite and border sprite
    SDL_Texture* spriteTexture = AssetManager::getInstance().getTexture("sprites");
    if (spriteTexture) {
        tileSprite = Sprite(spriteTexture, SPRITE_EMPTY);
        borderSprite = BorderSprite(spriteTexture, SPRITE_BORDER_CORNERS);
    }
}

Level::~Level() {
}

void Level::loadTestLevel() {
    // Fill entire level with zonks
    for (int y = 0; y < LEVEL_HEIGHT; y++) {
        for (int x = 0; x < LEVEL_WIDTH; x++) {
            tiles[y][x] = TileType::BASE;
        }
    }
    
    // Create a small empty area around the player starting position (5, 10)
    /* for (int y = 9; y <= 11; y++) {
        for (int x = 4; x <= 6; x++) {
            if (x >= 0 && x < LEVEL_WIDTH && y >= 0 && y < LEVEL_HEIGHT) {
                tiles[y][x] = TileType::EMPTY;
            }
        }
    } */
    
    // Add a few infotrons scattered around
    if (7 < LEVEL_HEIGHT && 15 < LEVEL_WIDTH) tiles[7][15] = TileType::INFOTRON;
    if (12 < LEVEL_HEIGHT && 20 < LEVEL_WIDTH) tiles[12][20] = TileType::INFOTRON;
    if (15 < LEVEL_HEIGHT && 25 < LEVEL_WIDTH) tiles[15][25] = TileType::INFOTRON;
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
                /* case TileType::WALL:
                    tileSprite.setSpriteId(SPRITE_WALL);
                    break; */
                case TileType::INFOTRON:
                    tileSprite.setSpriteId(SPRITE_INFOTRON);
                    break;
                case TileType::ZONK:
                    tileSprite.setSpriteId(SPRITE_ZONK);
                    break;
                case TileType::TERMINAL:
                    tileSprite.setSpriteId(SPRITE_TERMINAL);
                    break;
                case TileType::BASE:
                    tileSprite.setSpriteId(SPRITE_BASE);
                    break;
                case TileType::EXIT:
                    tileSprite.setSpriteId(SPRITE_EXIT);
                    break;
                case TileType::BUG:
                    tileSprite.setSpriteId(SPRITE_BUG);
                    break;
                case TileType::SNIK_SNAK:
                    tileSprite.setSpriteId(SPRITE_SIK_SNAK);
                    break;
                default:
                    continue;
            }
            
            tileSprite.render(renderer, x * TILE_SIZE, y * TILE_SIZE);
        }
    }
}

void Level::renderBorders(SDL_Renderer* renderer, int startX, int startY, int endX, int endY, float offsetX, float offsetY) {
    // Render borders for positions outside the 60x24 level area
    for (int y = startY; y < endY; y++) {
        for (int x = startX; x < endX; x++) {
            // Only render borders outside the level bounds
            bool isLeftBorder = (x == -1);
            bool isRightBorder = (x == LEVEL_WIDTH);
            bool isTopBorder = (y == -1);
            bool isBottomBorder = (y == LEVEL_HEIGHT);
            
            // Skip if this is inside the level and not a border
            if (!isLeftBorder && !isRightBorder && !isTopBorder && !isBottomBorder) {
                continue;
            }
            
            int spriteId;
            int quarter;
            bool shouldRender = true;
            
            // Determine border type and quarter based on position
            if (isLeftBorder && isTopBorder) {
                spriteId = SPRITE_BORDER_CORNERS;
                quarter = 0; // bottom right quarter
            } else if (isRightBorder && isTopBorder) {
                spriteId = SPRITE_BORDER_CORNERS;
                quarter = 1; // bottom left quarter
            } else if (isLeftBorder && isBottomBorder) {
                spriteId = SPRITE_BORDER_CORNERS;
                quarter = 2; // upper right quarter
            } else if (isRightBorder && isBottomBorder) {
                spriteId = SPRITE_BORDER_CORNERS;
                quarter = 3; // upper left quarter
            } else if (isTopBorder) {
                spriteId = SPRITE_BORDER_HORIZONTAL;
                quarter = 2; // top border quarter
            } else if (isBottomBorder) {
                spriteId = SPRITE_BORDER_HORIZONTAL;
                quarter = 0; // bottom border quarter
            } else if (isLeftBorder) {
                spriteId = SPRITE_BORDER_VERTICAL;
                quarter = 0; // left border quarter
            } else if (isRightBorder) {
                spriteId = SPRITE_BORDER_VERTICAL;
                quarter = 1; // right border quarter
            } else {
                shouldRender = false;
            }
            
            if (shouldRender) {
                int renderX = static_cast<int>((x * TILE_SIZE) + offsetX);
                int renderY = static_cast<int>((y * TILE_SIZE) + offsetY);
                borderSprite.renderWithSprite(renderer, renderX, renderY, spriteId, quarter);
            }
        }
    }
}

void Level::renderRegion(SDL_Renderer* renderer, int startX, int startY, int endX, int endY, float offsetX, float offsetY) {
    // Render borders first
    renderBorders(renderer, startX, startY, endX, endY, offsetX, offsetY);
    
    // Then render level tiles (normal coordinate system)
    for (int y = startY; y < endY; y++) {
        for (int x = startX; x < endX; x++) {
            // Only render tiles within the actual level bounds (60x24)
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
                case TileType::BASE:
                    tileSprite.setSpriteId(SPRITE_BASE);
                    break;
                case TileType::INFOTRON:
                    tileSprite.setSpriteId(SPRITE_INFOTRON);
                    break;
                case TileType::ZONK:
                    tileSprite.setSpriteId(SPRITE_ZONK);
                    break;
                case TileType::TERMINAL:
                    tileSprite.setSpriteId(SPRITE_TERMINAL);
                    break;
                case TileType::EXIT:
                    tileSprite.setSpriteId(SPRITE_EXIT);
                    break;
                case TileType::BUG:
                    tileSprite.setSpriteId(SPRITE_BUG);
                    break;
                case TileType::SNIK_SNAK:
                    tileSprite.setSpriteId(SPRITE_SIK_SNAK);
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
    // Add BASE as a walkable tile type
    return tile == TileType::EMPTY || tile == TileType::INFOTRON || tile == TileType::BASE;
}

TileType Level::getTile(int x, int y) const {
    if (x < 0 || x >= LEVEL_WIDTH || y < 0 || y >= LEVEL_HEIGHT) {
        return TileType::HARDWARE_8; // Non-walkable default
    }
    return tiles[y][x];
}

void Level::setTile(int x, int y, TileType type) {
    if (x >= 0 && x < LEVEL_WIDTH && y >= 0 && y < LEVEL_HEIGHT) {
        tiles[y][x] = type;
    }
}