# Create executable
add_executable(sdl-supaplex main.cpp)

# Link SDL2 libraries
target_link_libraries(sdl-supaplex ${SDL2_LIBRARIES})
target_include_directories(sdl-supaplex PRIVATE ${SDL2_INCLUDE_DIRS})
target_compile_options(sdl-supaplex PRIVATE ${SDL2_CFLAGS_OTHER})