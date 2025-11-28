#ifndef BASEOBJECT_HPP
#define BASEOBJECT_HPP

#include "GameObject.hpp"
#include <vector>

class BaseObject : public GameObject {
public:
    BaseObject(int x, int y);
    
    void update(float deltaTime) override;
    
    void startDigging();
    bool isDigging() const { return digging; }
    
private:
    bool digging;
    float animationTimer;
    float frameDuration;
    int currentFrame;
    bool animating;
    
    static const int SPRITE_BASE = 2;
    static const std::vector<int> DIGGING_SPRITES;
};

#endif // BASEOBJECT_HPP
