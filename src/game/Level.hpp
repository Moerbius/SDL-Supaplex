#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "../main.hpp"
#include "../systems/Sprite.hpp"
#include "../systems/BorderSprite.hpp"
#include "../entities/GameObject.hpp"
#include "../entities/BaseObject.hpp"
#include "../entities/InfotronObject.hpp"
#include "../entities/MurphyObject.hpp"
#include <array>
#include <vector>
#include <memory>

class Level {
public:
    Level();
    ~Level();
    
    void loadTestLevel();
    void update(float deltaTime);
    void render(SDL_Renderer* renderer);
    void renderRegion(SDL_Renderer* renderer, int startX, int startY, int endX, int endY, float offsetX, float offsetY);
    
    // Object management
    GameObject* getObjectAt(int x, int y) const;
    void removeObjectAt(int x, int y);
    void addObject(std::unique_ptr<GameObject> object);
    
    // Digging
    void digAt(int x, int y);
    bool isWalkable(int x, int y) const;
    
    // Murphy management
    MurphyObject* getMurphy() const { return murphy; }
    void spawnMurphy(int x, int y);
    
    static const int LEVEL_WIDTH = 60;
    static const int LEVEL_HEIGHT = 24;
    static const int TILE_SIZE = 16;
    
    // Border sprite IDs
    static const int SPRITE_BORDER_CORNERS = 229;
    static const int SPRITE_BORDER_VERTICAL = 230;
    static const int SPRITE_BORDER_HORIZONTAL = 231;

private:
    std::vector<std::unique_ptr<GameObject>> objects;
    MurphyObject* murphy; // Direct pointer for quick access
    BorderSprite borderSprite;
    
    void renderBorders(SDL_Renderer* renderer, int startX, int startY, int endX, int endY, float offsetX, float offsetY);
    void cleanupInactiveObjects();
};

#endif // LEVEL_HPP