cmake_minimum_required(VERSION 3.16)

project(Project02 LANGUAGES CXX)

# Require C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find SDL2
find_package(PkgConfig REQUIRED)
find_package(tinyxml2 CONFIG REQUIRED)

# Find SDL2 and SDL2_image
pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED IMPORTED_TARGET SDL2_image)

# Create executable
add_executable(Project02
        src/main.cpp
        src/Engine.cpp
        src/Input.cpp
        src/Textures.cpp
)

# Link SDL2
target_link_libraries(Project02 PRIVATE
        PkgConfig::SDL2
        PkgConfig::SDL2_IMAGE
        tinyxml2::tinyxml2
)