#ifndef MAIN_HPP
#define MAIN_HPP

#include <SDL2/SDL.h>
#include <iostream>
#include <memory>
#include <vector>

// Forward declarations
class Game;
class Level;
class GameObject;
class MurphyObject;  // Add MurphyObject instead of Player
class Renderer;
class InputManager;

// Game states
enum class GameState {
    MENU,
    PLAYING,
    PAUSED,
    GAME_OVER
};

// Tile types for Supaplex
enum class TileType {
    EMPTY,
    PLAYER,
    INFOTRON,
    TERMINAL,
    BASE,
    
    EXIT,

    BUG,
    SNIK_SNAK,
    ZONK,

    DISK_ORANGE,
    DISK_YELLOW,
    DISK_RED,

    PORT_1,
    PORT_2,
    PORT_3,


    CHIP_1,
    CHIP_2,
    CHIP_3,

    HARDWARE_1,
    HARDWARE_2,
    HARDWARE_3,
    HARDWARE_4,
    HARDWARE_5,
    HARDWARE_6,
    HARDWARE_7,
    HARDWARE_8,
    HARDWARE_9,
    HARDWARE_10
    
    /* DISK,
    PORT,
    
    YELLOW_DISK,
    
    RED_DISK,
    ELECTRON */
    
};

#endif // MAIN_HPP