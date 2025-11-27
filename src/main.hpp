#ifndef MAIN_HPP
#define MAIN_HPP

#include <SDL2/SDL.h>
#include <iostream>
#include <memory>
#include <vector>

// Forward declarations
class Game;
class Level;
class Player;
class GameObject;
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
    WALL,
    BASE,
    INFOTRON,
    CHIP,
    HARDWARE,
    PLAYER,
    ZONK,
    DISK,
    PORT,
    SNIK_SNAK,
    YELLOW_DISK,
    TERMINAL,
    RED_DISK,
    ELECTRON,
    BUG
};

#endif // MAIN_HPP