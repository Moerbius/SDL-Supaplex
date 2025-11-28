#include "Level.hpp"
#include "../systems/AssetManager.hpp"
#include <algorithm>
#include <random>

Level::Level() : murphy(nullptr) {
    // Initialize border sprite
    SDL_Texture* spriteTexture = AssetManager::getInstance().getTexture("sprites");
    if (spriteTexture) {
        borderSprite = BorderSprite(spriteTexture, SPRITE_BORDER_CORNERS);
    }
}

Level::~Level() {
}

void Level::loadTestLevel() {
    objects.clear();
    murphy = nullptr;
    
    // Random number generation
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    
    // Fill level with randomly placed BASE and INFOTRON objects
    for (int y = 0; y < LEVEL_HEIGHT; y++) {
        for (int x = 0; x < LEVEL_WIDTH; x++) {
            // Skip the area around Murphy's starting position (5, 10)
            if (x >= 4 && x <= 6 && y >= 9 && y <= 11) {
                continue; // Leave empty space around Murphy
            }
            
            double random = dis(gen);
            
            if (random < 0.7) {
                // 70% chance for BASE object
                objects.push_back(std::make_unique<BaseObject>(x, y));
            } else if (random < 0.85) {
                // 15% chance for INFOTRON object
                objects.push_back(std::make_unique<InfotronObject>(x, y));
            }
            // 15% chance for empty space (no object created)
        }
    }
    
    // Spawn Murphy in the cleared area
    spawnMurphy(5, 10);
}

void Level::spawnMurphy(int x, int y) {
    auto murphyObj = std::make_unique<MurphyObject>(x, y);
    murphy = murphyObj.get(); // Keep direct pointer
    objects.push_back(std::move(murphyObj));
}

void Level::update(float deltaTime) {
    // Process Murphy's input first
    if (murphy && murphy->isActive()) {
        murphy->processInput(this);
    }
    
    // Update all objects
    for (auto& object : objects) {
        if (object && object->isActive()) {
            object->update(deltaTime);
        }
    }
    
    // Clean up inactive objects (but check if Murphy was removed)
    cleanupInactiveObjects();
}

GameObject* Level::getObjectAt(int x, int y) const {
    for (const auto& object : objects) {
        if (object && object->isActive() && object->getX() == x && object->getY() == y) {
            return object.get();
        }
    }
    return nullptr;
}

void Level::removeObjectAt(int x, int y) {
    for (auto& object : objects) {
        if (object && object->getX() == x && object->getY() == y) {
            object->setActive(false);
            break;
        }
    }
}

void Level::addObject(std::unique_ptr<GameObject> object) {
    objects.push_back(std::move(object));
}

void Level::digAt(int x, int y) {
    GameObject* obj = getObjectAt(x, y);
    if (!obj) return;
    
    if (obj->getType() == ObjectType::BASE) {
        BaseObject* baseObj = static_cast<BaseObject*>(obj);
        baseObj->startDigging();
    } else if (obj->getType() == ObjectType::INFOTRON) {
        InfotronObject* infoObj = static_cast<InfotronObject*>(obj);
        infoObj->collect();  // Revert back to collect()
    }
}

bool Level::isWalkable(int x, int y) const {
    if (x < 0 || x >= LEVEL_WIDTH || y < 0 || y >= LEVEL_HEIGHT) {
        return false;
    }
    
    GameObject* obj = getObjectAt(x, y);
    if (!obj) return true; // Empty space
    
    // Can walk on BASE and INFOTRON (they get collected/dug)
    // But check if infotron is already collecting
    if (obj->getType() == ObjectType::INFOTRON) {
        InfotronObject* infoObj = static_cast<InfotronObject*>(obj);
        return !infoObj->isCollecting(); // Can only walk on if not already collecting
    }
    
    return obj->getType() == ObjectType::BASE;
}

void Level::renderRegion(SDL_Renderer* renderer, int startX, int startY, int endX, int endY, float offsetX, float offsetY) {
    // Render borders first
    renderBorders(renderer, startX, startY, endX, endY, offsetX, offsetY);
    
    // Render all active objects in the region
    for (const auto& object : objects) {
        if (!object || !object->isActive()) continue;
        
        int objX = object->getX();
        int objY = object->getY();
        
        // Check if object is in visible region
        if (objX >= startX && objX < endX && objY >= startY && objY < endY) {
            object->render(renderer, offsetX, offsetY);
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
                quarter = 0;
            } else if (isRightBorder && isTopBorder) {
                spriteId = SPRITE_BORDER_CORNERS;
                quarter = 1;
            } else if (isLeftBorder && isBottomBorder) {
                spriteId = SPRITE_BORDER_CORNERS;
                quarter = 2;
            } else if (isRightBorder && isBottomBorder) {
                spriteId = SPRITE_BORDER_CORNERS;
                quarter = 3;
            } else if (isTopBorder) {
                spriteId = SPRITE_BORDER_HORIZONTAL;
                quarter = 2;
            } else if (isBottomBorder) {
                spriteId = SPRITE_BORDER_HORIZONTAL;
                quarter = 0;
            } else if (isLeftBorder) {
                spriteId = SPRITE_BORDER_VERTICAL;
                quarter = 0;
            } else if (isRightBorder) {
                spriteId = SPRITE_BORDER_VERTICAL;
                quarter = 1;
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

void Level::cleanupInactiveObjects() {
    // Check if Murphy becomes inactive
    if (murphy && !murphy->isActive()) {
        murphy = nullptr;
    }
    
    // Remove inactive objects
    objects.erase(
        std::remove_if(objects.begin(), objects.end(),
            [](const std::unique_ptr<GameObject>& obj) {
                return !obj || !obj->isActive();
            }),
        objects.end());
}