#ifndef INFOTRONOBJECT_HPP
#define INFOTRONOBJECT_HPP

#include "GameObject.hpp"
#include <vector>

class InfotronObject : public GameObject {
public:
    InfotronObject(int x, int y);
    
    void update(float deltaTime) override;
    
    void collect();
    bool isCollected() const { return collected; }
    bool isCollecting() const { return collecting; }  // Add collecting state check
    
private:
    bool collected;
    bool collecting;  // Track if currently playing collection animation
    float animationTimer;
    float frameDuration;
    int currentFrame;
    bool animating;
    
    static const int SPRITE_INFOTRON = 4;
    static const std::vector<int> COLLECTING_SPRITES;  // Animation sprites 121-127
};

#endif // INFOTRONOBJECT_HPP
