#include "LevelLoader.hpp"
#include "Level.hpp"
#include "../entities/BaseObject.hpp"
#include "../entities/InfotronObject.hpp"
#include "../entities/ZonkObject.hpp"
#include "../entities/ChipObject.hpp"
#include <fstream>
#include <iostream>

std::vector<LevelData> LevelLoader::levels;
int LevelLoader::levelCount = 0;

bool LevelLoader::loadLevelsFile(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open levels file: " << filePath << std::endl;
        return false;
    }
    
    // Get file size
    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    
    // Calculate number of levels (1536 bytes each)
    levelCount = fileSize / 1536;
    std::cout << "Found " << levelCount << " levels in " << filePath << std::endl;
    
    // Read all level data
    std::vector<uint8_t> fileData(fileSize);
    file.read(reinterpret_cast<char*>(fileData.data()), fileSize);
    file.close();
    
    // Parse each level
    levels.clear();
    levels.reserve(levelCount);
    
    for (int i = 0; i < levelCount; i++) {
        size_t offset = i * 1536;
        LevelData levelData = parseLevelData(fileData, offset);
        levels.push_back(levelData);
    }
    
    return true;
}

bool LevelLoader::loadLevel(Level* level, int levelNumber) {
    if (levelNumber < 1 || levelNumber > levelCount) {
        std::cerr << "Invalid level number: " << levelNumber << std::endl;
        return false;
    }
    
    const LevelData& levelData = levels[levelNumber - 1];  // Convert to 0-based index
    
    // Clear existing objects
    level->clearAllObjects();
    
    // Create objects from tile data
    int murphyX = -1, murphyY = -1;
    
    // Start from row 1 and column 1 to shift level up and left by 1 tile each
    for (int y = 1; y < 24; y++) {
        for (int x = 1; x < 60; x++) {  // Start from x=1 instead of x=0
            int index = y * 60 + x;
            uint8_t tileValue = levelData.tileData[index];
            
            // Debug: Log first few tiles to see what we're reading
            if (x < 11 && y < 4) {  // Adjust debug range for shifted coordinates
                std::cout << "Tile at (" << x << "," << y << "): 0x" << std::hex << (int)tileValue << std::dec << std::endl;
            }
            
            // Handle Murphy separately since he needs special spawning
            if (tileValue == 0x03) {  // Murphy tile
                murphyX = x - 1;  // Adjust Murphy's X position for the shift
                murphyY = y - 1;  // Adjust Murphy's Y position for the shift
                continue;
            }
            
            // Create appropriate game object at shifted position
            auto object = createObjectFromTile(tileValue, x - 1, y - 1);  // Shift both X and Y positions
            if (object) {
                level->addObject(std::move(object));
            }
        }
    }
    
    // Spawn Murphy at the found position
    if (murphyX >= 0 && murphyY >= 0) {
        std::cout << "Spawning Murphy at position (" << murphyX << ", " << murphyY << ")" << std::endl;
        level->spawnMurphy(murphyX, murphyY);
    } else {
        std::cerr << "Warning: No Murphy starting position found in level " << levelNumber << std::endl;
        std::cout << "Using fallback Murphy position (5, 10)" << std::endl;
        level->spawnMurphy(5, 10);  // Fallback position
    }
    
    std::cout << "Loaded level " << levelNumber << ": \"" << levelData.title << "\"" << std::endl;
    return true;
}

std::string LevelLoader::getLevelTitle(int levelNumber) {
    if (levelNumber < 1 || levelNumber > levelCount) {
        return "Invalid Level";
    }
    return levels[levelNumber - 1].title;
}

LevelData LevelLoader::parseLevelData(const std::vector<uint8_t>& rawData, size_t offset) {
    LevelData data;
    
    // Extract tile data (first 1440 bytes = 60x24)
    data.tileData.resize(1440);
    for (int i = 0; i < 1440; i++) {
        data.tileData[i] = rawData[offset + i];
    }
    
    // Extract level properties
    data.gravity = (rawData[offset + 1444] != 0);
    data.freezeZonks = (rawData[offset + 1469] != 0);
    data.infrotronsNeeded = rawData[offset + 1470];
    
    // Extract title (23 characters, space-padded)
    data.title.clear();
    for (int i = 0; i < 23; i++) {
        char c = rawData[offset + 1446 + i];
        if (c != ' ') {  // Skip trailing spaces
            data.title += c;
        } else if (!data.title.empty()) {
            data.title += c;  // Keep spaces in the middle
        }
    }
    
    return data;
}

ObjectType LevelLoader::tileToObjectType(uint8_t tileValue) {
    switch (tileValue) {
        case 0x00: return ObjectType::PLAYER;  // Empty - no object created
        case 0x01: return ObjectType::BASE;
        case 0x02: return ObjectType::ZONK;
        case 0x03: return ObjectType::PLAYER;  // Murphy - handled separately
        case 0x04: return ObjectType::INFOTRON;
        case 0x05: return ObjectType::CHIP_1;
        case 0x06: return ObjectType::HARDWARE_1;  // Wall
        case 0x07: return ObjectType::EXIT;
        case 0x08: return ObjectType::DISK_ORANGE;
        case 0x11: return ObjectType::SNIK_SNAK;
        case 0x13: return ObjectType::TERMINAL;
        case 0x18: return ObjectType::BUG;  // Electron -> Bug for now
        case 0x19: return ObjectType::BUG;
        default:
            return ObjectType::BASE;  // Default to BASE instead of HARDWARE_1 for now
    }
}

std::unique_ptr<GameObject> LevelLoader::createObjectFromTile(uint8_t tileValue, int x, int y) {
    switch (tileValue) {
        case 0x00:  // Empty - no object
            return nullptr;
        case 0x01:  // Zonk
            return std::make_unique<ZonkObject>(x, y);
        case 0x02:  // Base  
            return std::make_unique<BaseObject>(x, y);
        
        case 0x04:  // Infotron
            return std::make_unique<InfotronObject>(x, y);
        case 0x05:  // RAM chip - now implemented
            return std::make_unique<ChipObject>(x, y);
        case 0x06:  // Wall - not implemented yet
            return nullptr;
        case 0x07:  // Exit - not implemented yet
            return nullptr;
        // Add more cases as objects are implemented
        default:
            return nullptr;  // Don't create any object for unimplemented tiles
    }
}
