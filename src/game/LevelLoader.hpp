#ifndef LEVELLOADER_HPP
#define LEVELLOADER_HPP

#include "../main.hpp"
#include "../entities/GameObject.hpp"  // Add this include for ObjectType
#include <string>
#include <vector>
#include <memory>

class Level;

struct LevelData {
    std::vector<uint8_t> tileData;  // 60x24 tile array
    bool gravity;
    std::string title;
    bool freezeZonks;
    uint8_t infrotronsNeeded;
    int murphyStartX, murphyStartY;
};

class LevelLoader {
public:
    static bool loadLevelsFile(const std::string& filePath);
    static bool loadLevel(Level* level, int levelNumber);  // 1-based level number
    static int getLevelCount() { return levelCount; }
    static std::string getLevelTitle(int levelNumber);
    
private:
    static std::vector<LevelData> levels;
    static int levelCount;
    
    static ObjectType tileToObjectType(uint8_t tileValue);
    static std::unique_ptr<GameObject> createObjectFromTile(uint8_t tileValue, int x, int y);
    static LevelData parseLevelData(const std::vector<uint8_t>& rawData, size_t offset);
};

#endif // LEVELLOADER_HPP
