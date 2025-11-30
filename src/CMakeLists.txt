# Create executable
add_executable(sdl-supaplex 
    main.cpp
    game/Level.cpp
    game/Game.cpp
    game/LevelLoader.cpp
    entities/MurphyObject.cpp
    entities/GameObject.cpp
    entities/BaseObject.cpp
    entities/InfotronObject.cpp
    entities/ZonkObject.cpp
    entities/ChipObject.cpp
    systems/AssetManager.cpp
    systems/Sprite.cpp
    systems/BorderSprite.cpp
)

# Link SDL2 libraries
target_link_libraries(sdl-supaplex ${SDL2_LIBRARIES} SDL2_image)
target_include_directories(sdl-supaplex PRIVATE ${SDL2_INCLUDE_DIRS})
target_compile_options(sdl-supaplex PRIVATE ${SDL2_CFLAGS_OTHER})