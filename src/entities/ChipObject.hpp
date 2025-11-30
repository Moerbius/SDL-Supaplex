#ifndef CHIPOBJECT_HPP
#define CHIPOBJECT_HPP

#include "GameObject.hpp"

class ChipObject : public GameObject {
public:
    ChipObject(int x, int y);
    
    void update(float deltaTime) override;
    
    // Chips can be collected like infotrons in some variants
    void collect();
    bool isCollected() const { return collected; }
    
private:
    bool collected;
    
    static const int SPRITE_CHIP = 5;  // RAM chip sprite
};

#endif // CHIPOBJECT_HPP
