cmake_minimum_required(VERSION 3.16)

project(DungeonExplorer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)

find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

add_library(GraphCore
    src/game/Graph.cpp
)

target_include_directories(GraphCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/game
)

target_link_libraries(GraphCore PUBLIC
    SFML::Graphics
    SFML::Window
    SFML::System
)

add_executable(DungeonExplorer
    src/main.cpp
    src/ui/Menu.cpp
    src/game/Game.cpp
    src/game/rendering/DungeonRenderer.cpp
    src/game/ui/SettingsWindow.cpp
)

target_link_libraries(DungeonExplorer PRIVATE
    GraphCore
    SFML::Graphics
    SFML::Window
    SFML::System
)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()