#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include "../main.hpp"
#include "../systems/Sprite.hpp"
#include <vector>

enum class ObjectType {
    PLAYER,
    BASE,
    INFOTRON,
    ZONK,
    TERMINAL,
    EXIT,
    BUG,
    SNIK_SNAK
};

class GameObject {
public:
    GameObject(int x, int y, ObjectType type);
    virtual ~GameObject() = default;
    
    virtual void update(float deltaTime) {}
    virtual void render(SDL_Renderer* renderer, float offsetX, float offsetY);
    
    // Position
    int getX() const { return x; }
    int getY() const { return y; }
    void setPosition(int newX, int newY) { x = newX; y = newY; }
    
    // Type
    ObjectType getType() const { return type; }
    
    // State
    bool isActive() const { return active; }
    void setActive(bool state) { active = state; }
    
protected:
    void setSpriteId(int spriteId);
    
    int x, y;
    ObjectType type;
    bool active;
    
    Sprite sprite;
    
    static const int TILE_SIZE = 16;
};

#endif // GAMEOBJECT_HPP
