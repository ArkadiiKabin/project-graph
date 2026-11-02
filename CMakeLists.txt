cmake_minimum_required(VERSION 3.16)

project(DungeonExplorer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

add_executable(DungeonExplorer
    src/main.cpp
    src/ui/Menu.cpp
    src/game/Game.cpp
    src/game/Graph.cpp
)

target_link_libraries(DungeonExplorer
    SFML::Graphics
    SFML::Window
    SFML::System
)