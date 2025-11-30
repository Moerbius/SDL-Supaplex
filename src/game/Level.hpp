#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "../main.hpp"
#include "../systems/Sprite.hpp"
#include "../systems/BorderSprite.hpp"
#include "../entities/GameObject.hpp"
#include "../entities/BaseObject.hpp"
#include "../entities/InfotronObject.hpp"
#include "../entities/ZonkObject.hpp"
#include "../entities/ChipObject.hpp"
#include "../entities/MurphyObject.hpp"
#include <array>
#include <vector>
#include <memory>

class Level {
public:
    Level();
    ~Level();
    
    void loadTestLevel();
    bool loadFromFile(int levelNumber);  // Load level from LEVELS.DAT
    void clearAllObjects();  // Clear all objects except borders
    
    void update(float deltaTime);
    void render(SDL_Renderer* renderer);
    void renderRegion(SDL_Renderer* renderer, int startX, int startY, int endX, int endY, float offsetX, float offsetY);
    
    // Object management
    GameObject* getObjectAt(int x, int y) const;
    void removeObjectAt(int x, int y);
    void addObject(std::unique_ptr<GameObject> object);
    void moveObject(GameObject* obj, int newX, int newY);  // Add this for gravity
    
    // Digging
    void digAt(int x, int y);
    bool isWalkable(int x, int y) const;
    
    // Murphy management
    MurphyObject* getMurphy() const { return murphy; }
    void spawnMurphy(int x, int y);
    
    // Gravity system
    void triggerGravityCheckAbove(int x, int y);  // Moved to public section
    
    static constexpr int LEVEL_WIDTH = 58;  // Changed from 60 to 58
    static constexpr int LEVEL_HEIGHT = 22; // Changed from 24 to 22
    static constexpr int TILE_SIZE = 16;
    
    // Border sprite IDs
    static constexpr int SPRITE_BORDER_CORNERS = 229;
    static constexpr int SPRITE_BORDER_VERTICAL = 230;
    static constexpr int SPRITE_BORDER_HORIZONTAL = 231;

private:
    std::vector<std::unique_ptr<GameObject>> objects;
    MurphyObject* murphy; // Direct pointer for quick access
    BorderSprite borderSprite;
    
    void renderBorders(SDL_Renderer* renderer, int startX, int startY, int endX, int endY, float offsetX, float offsetY);
    void cleanupInactiveObjects();
};

#endif // LEVEL_HPP