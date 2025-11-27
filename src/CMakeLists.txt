# Create executable
add_executable(sdl-supaplex 
    main.cpp
    game/Game.cpp
    game/Level.cpp
    entities/Player.cpp
    systems/AssetManager.cpp
    systems/Sprite.cpp
)

# Link SDL2 libraries
target_link_libraries(sdl-supaplex ${SDL2_LIBRARIES} SDL2_image)
target_include_directories(sdl-supaplex PRIVATE ${SDL2_INCLUDE_DIRS})
target_compile_options(sdl-supaplex PRIVATE ${SDL2_CFLAGS_OTHER})