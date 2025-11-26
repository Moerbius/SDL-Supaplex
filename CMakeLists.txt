cmake_minimum_required(VERSION 3.10)

# Define project name
project(sdl-supaplex VERSION 1.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SDL2
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)

# Set up required subdirectories
add_subdirectory(src)