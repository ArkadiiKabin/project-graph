# Dungeon Explorer

<p align="center">
  <img src="screenshots/dungeonBase.png" alt="Dungeon Explorer - Generated Dungeon" width="800">
</p>

> **Разработка игровой системы генерации подземелий и поиска маршрутов
> на основе графов**\
> *Procedural Dungeon Generation and Graph-Based Pathfinding System*

A C++17 application for procedural dungeon generation, graph
construction, and visual pathfinding. The project combines randomly
generated dungeon layouts with classic graph traversal and shortest-path
algorithms, allowing users to explore how graph structures and
algorithms behave in a visual environment.

------------------------------------------------------------------------

## Table of Contents

-   [Overview](#overview)
-   [Project Goals](#project-goals)
-   [Features](#features)
-   [Explorer Mode](#explorer-mode)
-   [Procedural Dungeon Generation](#procedural-dungeon-generation)
-   [Graph Model](#graph-model)
-   [Pathfinding Algorithms](#pathfinding-algorithms)
-   [Generation Settings](#generation-settings)
-   [Visual System](#visual-system)
-   [Project Architecture](#project-architecture)
-   [Technologies](#technologies)
-   [Requirements](#requirements)
-   [Build and Run](#build-and-run)
-   [Using the Application](#using-the-application)
-   [Project Roadmap](#project-roadmap)
-   [Planned Game Modes](#planned-game-modes)
-   [Current Project Status](#current-project-status)

------------------------------------------------------------------------

## Overview

Dungeon Explorer is a graphical application focused on procedural
dungeon generation and graph-based pathfinding.

Each generated dungeon consists of rooms connected by passages. The room
layout forms the visual representation of a graph: rooms act as
vertices, while passages between rooms act as edges. Connections can
have weights, which are used by weighted pathfinding algorithms.

The application makes it possible to generate different dungeon layouts,
configure generation parameters, inspect the resulting graph, and
compare the routes found by several well-known algorithms.

The project is designed both as an interactive application and as a
practical demonstration of graph theory, procedural generation, and
pathfinding.

## Project Goals

The main goals of the project are:

-   Develop a system for procedural generation of dungeon layouts
-   Represent generated dungeons as connected graphs
-   Implement and demonstrate graph traversal and shortest-path
    algorithms
-   Visualize graph connections, edge weights, and calculated routes
-   Provide configurable generation parameters
-   Build a foundation for future gameplay-oriented features

## Features

### Implemented

-   Procedural generation of dungeon rooms
-   Randomized room count within configurable limits
-   Grid-based room placement
-   Orthogonal connections between adjacent rooms
-   Connected dungeon graph generation
-   Prevention of duplicate connections
-   Configurable graph density
-   Random edge weights within a configurable range
-   BFS, DFS, and Dijkstra pathfinding algorithms
-   Visual highlighting of calculated routes
-   Separate visual styles for different pathfinding algorithms
-   Multiple room texture sets
-   Weighted corridor visualization
-   Automatic dungeon fitting within the view
-   Generation settings window with Apply, Cancel, and Reset actions

------------------------------------------------------------------------

## Explorer Mode

Explorer Mode is the currently implemented application mode. It focuses
on generating dungeons and exploring their underlying graph structure.

The user can generate a dungeon, adjust generation parameters, and run
pathfinding algorithms on the resulting graph. The application displays
the dungeon layout, its passages, connection weights, and the route
produced by the selected algorithm.

Explorer Mode includes:

1.  **Dungeon generation** --- creates a new randomized room layout.
2.  **Graph construction** --- connects orthogonally adjacent rooms.
3.  **Pathfinding** --- runs BFS, DFS, or Dijkstra on the generated
    graph.
4.  **Route visualization** --- highlights the route found by the
    selected algorithm.
5.  **Generation configuration** --- allows the user to adjust room
    count, edge weights, and graph density.

Explorer Mode is considered functionally complete for the current
project stage. Future gameplay systems are planned as a separate mode
rather than as additional complexity in this mode.

## Procedural Dungeon Generation

Dungeon generation is based on a logical two-dimensional grid.

### Room placement

-   The first room is placed at the starting grid coordinate `(0, 0)`.
-   Additional rooms are placed one at a time.
-   Each new room is placed in an available orthogonal cell next to an
    already existing room.
-   The generation process continues until the randomly selected room
    count is reached.
-   The room count is controlled by the generation settings.

This growth-based approach creates a connected layout during room
placement and avoids placing rooms on the same grid cell.

### Room connections

After room placement, the system constructs graph connections between
rooms.

Only orthogonal neighbours can be connected:

-   Up
-   Down
-   Left
-   Right

Diagonal room positions do not create diagonal graph connections.

Connection generation is performed in two stages:

1.  **Connectivity stage** --- each room added after the starting room
    is connected to an earlier orthogonal neighbour. This preserves
    connectivity of the generated graph.
2.  **Additional connections stage** --- the system checks orthogonally
    adjacent room pairs and may add extra connections according to the
    configured graph density.

Existing connections are not duplicated.

Connectivity has priority over the optional density limit: the system
must preserve a connected dungeon even when a mandatory connection
causes a room to exceed the preferred maximum degree.

### Connection weights

Each graph edge receives a random weight from the configured minimum and
maximum values.

These weights are used by Dijkstra's algorithm and are also displayed in
the dungeon view.

------------------------------------------------------------------------

## Graph Model

The generated dungeon is represented as an undirected weighted graph.

  Dungeon element      Graph element
  -------------------- ---------------------
  Room                 Vertex (node)
  Passage / corridor   Edge
  Corridor weight      Edge weight
  Dungeon layout       Graph visualization

The graph is undirected because a passage connects two rooms in both
directions.

The graph is weighted because every connection has a numerical cost.
This allows the application to demonstrate both unweighted traversal
algorithms and weighted shortest-path search.

The generation process aims to produce a connected graph, so every
generated room can be reached from the starting room.

## Pathfinding Algorithms

The application implements three graph algorithms.

### Breadth-First Search (BFS)

Breadth-First Search explores the graph level by level, visiting
neighbouring vertices before moving farther away from the starting
vertex.

-   Uses a queue
-   Finds a path with the minimum number of edges in an unweighted graph
-   Does not use corridor weights when choosing the route

**Visualization:** BFS routes are highlighted in yellow.

![BFS pathfinding result](screenshots/bfsTry.png)

### Depth-First Search (DFS)

Depth-First Search explores as far as possible along one branch before
backtracking.

-   Uses a stack or recursive traversal
-   Explores the graph depth-first
-   Finds a valid route when one exists, but does not guarantee the
    route with the fewest edges or the lowest total weight

**Visualization:** DFS routes are highlighted in red.

![DFS pathfinding result](screenshots/dfsTry.png)

### Dijkstra's Algorithm

Dijkstra's algorithm finds a minimum-total-weight path from the starting
vertex to the destination when edge weights are non-negative.

-   Takes corridor weights into account
-   Maintains the best known distance to each vertex
-   Updates routes when a lower-cost path is found

**Visualization:** Dijkstra routes are highlighted in orange.

![Dijkstra pathfinding result](screenshots/dijTry.png)

### Comparing the algorithms

  -----------------------------------------------------------------------
  Algorithm         Uses edge weights Main search       Route guarantee
                                      principle         
  ----------------- ----------------- ----------------- -----------------
  BFS               No                Level-by-level    Minimum number of
                                      traversal         edges

  DFS               No                Depth-first       A valid route,
                                      traversal         not necessarily
                                                        optimal

  Dijkstra          Yes               Minimum           Minimum total
                                      accumulated cost  weight for
                                                        non-negative
                                                        weights
  -----------------------------------------------------------------------

The routes may differ because the algorithms optimize or prioritize
different things. In particular, the route with the fewest connections
is not necessarily the route with the lowest total weight.

------------------------------------------------------------------------

## Generation Settings

Dungeon generation can be configured through the Settings window.

### Room count

Controls the minimum and maximum number of rooms that may be generated.

-   Minimum Rooms
-   Maximum Rooms

The generated room count is selected randomly within the configured
inclusive range.

### Path weight

Controls the range of random weights assigned to connections.

-   Minimum Path Weight
-   Maximum Path Weight

The minimum value must not exceed the maximum value.

### Graph density

Controls how many additional connections may be generated between
adjacent rooms.

  Setting     Value Description
  --------- ------- ------------------------------------
  Low             1 Lower target connection density
  Medium          2 Moderate target connection density
  High            3 Higher target connection density

The density value is used to determine the preferred maximum number of
connections per room for optional connections. Mandatory connections
required to preserve graph connectivity take priority.

### Applying settings

The generation settings interface is shown below.

![Dungeon generation settings](screenshots/settingsDungeon.png)

The settings window provides three actions:

-   **Apply** --- validates and applies the configured values.
-   **Cancel** --- closes the settings window without applying the
    current draft.
-   **Reset** --- restores the default values in the settings form. The
    reset values must still be applied.

------------------------------------------------------------------------

## Visual System

The visual layer presents the generated graph as a dungeon.

### Room appearance

The project includes five room texture sets:

-   `room_1` --- pink
-   `room_2` --- green
-   `room_3` --- blue
-   `room_4` --- grey
-   `room_5` --- brown

Each room texture set contains a base room image and transparent
overlays for passages on the top, left, bottom, and right sides.

### Corridors

Connections between rooms are rendered using corridor textures. The
ordinary corridor appearance is selected according to the texture set
associated with the room.

### Path visualization

The application uses distinct corridor textures to distinguish algorithm
results:

  Algorithm   Route colour
  ----------- --------------
  BFS         Yellow
  DFS         Red
  Dijkstra    Orange

Only the currently selected algorithm route is displayed at a time.
Running another algorithm replaces the previous highlighted route.

### Dungeon view

The dungeon is fitted to the available view so that generated layouts
can be inspected even when their dimensions vary.

------------------------------------------------------------------------

## Project Architecture

The project is organized into UI and game-logic components.

``` text
project-graph/
├── CMakeLists.txt
├── assets/
│   ├── fonts/
│   │   └── HS_LunaObscura.ttf
│   └── textures/
│       ├── room_1/
│       ├── room_2/
│       ├── room_3/
│       ├── room_4/
│       ├── room_5/
│       └── roads/
└── src/
    ├── main.cpp
    ├── ui/
    │   ├── Menu.h
    │   └── Menu.cpp
    └── game/
        ├── Game.hpp
        ├── Game.cpp
        ├── Graph.hpp
        └── Graph.cpp
```

### Main components

  ---------------------------------------------------------------------
  File                               Responsibility
  ---------------------------------- ----------------------------------
  `main.cpp`                         Application entry point

  `ui/Menu.h` / `ui/Menu.cpp`        Main menu interface

  `game/Game.hpp` / `game/Game.cpp`  Main game/application loop,
                                     rendering, input handling, and
                                     application state

  `game/Graph.hpp` /                 Graph data, dungeon generation,
  `game/Graph.cpp`                   connections, weights, and
                                     pathfinding

  `CMakeLists.txt`                   CMake build configuration

  `assets/`                          Fonts and visual resources
  ---------------------------------------------------------------------

The architecture separates the graphical application layer from the
graph and generation logic.

------------------------------------------------------------------------

## Technologies

-   **C++17** --- primary programming language
-   **SFML 3.0.2** --- graphics, window management, and input handling
-   **CMake** --- project configuration and build system
-   **Git** --- version control

## Requirements

To build the project, you need:

-   A C++ compiler with C++17 support
-   CMake
-   SFML 3.0.2 development libraries
-   Git (optional, for cloning the repository)

The exact SFML installation procedure depends on the operating system
and package manager.

## Build and Run

The following commands are intended for a Linux environment with the
required dependencies installed.

### 1. Clone the repository

``` bash
git clone https://github.com/ArkadiiKabin/project-graph.git
cd project-graph
```

### 2. Configure the project

``` bash
cmake -S . -B build
```

### 3. Build

``` bash
cmake --build build
```

### 4. Run

``` bash
./build/DungeonExplorer
```

If CMake cannot find SFML, make sure the SFML 3.0.2 development package
is installed and available to CMake.

------------------------------------------------------------------------

## Using the Application

A typical Explorer Mode session follows this sequence:

1.  Launch the application.
2.  Open the dungeon generation settings if you want to change the
    defaults.
3.  Set the room count range, path weight range, and graph density.
4.  Apply the settings.
5.  Generate a dungeon.
6.  Select a pathfinding algorithm.
7.  Inspect the highlighted route and the generated dungeon graph.
8.  Generate another dungeon to explore a different layout.

The exact on-screen controls are defined by the application interface.

------------------------------------------------------------------------

## Project Roadmap

The current focus is to preserve the completed Explorer Mode
functionality and develop a separate gameplay-oriented mode.

  Feature                                     Status
  ------------------------------------------- -------------
  Explorer Mode                               Complete
  Procedural room generation                  Complete
  Connected graph generation                  Complete
  BFS pathfinding                             Complete
  DFS pathfinding                             Complete
  Dijkstra pathfinding                        Complete
  Generation settings                         Complete
  Route and corridor visualization            Complete
  Adventure Mode                              Planned
  Additional room types and gameplay events   Planned
  Keys, doors, and locked rooms               Planned
  Multiple dungeon floors                     Planned
  Boss encounters and progression             Planned
  Dungeon Editor                              Future idea

Planned features describe the intended direction of development and are
not part of the currently implemented feature set.

## Planned Game Modes

### Adventure Mode

Adventure Mode is planned as a gameplay-focused extension of the
project. Instead of only generating and analyzing a dungeon, the player
will be able to explore it as a game world.

Potential features include:

-   A player character and step-by-step dungeon exploration
-   Different room types, such as ordinary rooms, monster rooms,
    treasure rooms, and healing rooms
-   Locked rooms and keys
-   Multiple dungeon floors
-   Random events during exploration
-   Increasing difficulty as the player progresses
-   Boss encounters at progression milestones, with a boss planned every
    five levels

These mechanics are planned and may change during development.

### Dungeon Editor

Dungeon Editor is a possible separate mode focused on manual dungeon
construction.

The intended idea is to let users create and edit dungeon layouts
themselves rather than relying exclusively on procedural generation. The
exact editor tools and workflow have not yet been defined.

------------------------------------------------------------------------

## Current Project Status

The project currently provides a working procedural dungeon generator
and a visual graph pathfinding environment.

The completed Explorer Mode demonstrates:

-   How a dungeon layout can be represented as a graph
-   How connected graph structures can be generated procedurally
-   How edge weights affect shortest-path search
-   How BFS, DFS, and Dijkstra explore the same graph differently
-   How graph algorithms can be visualized in an interactive application

The next major development direction is Adventure Mode, which will build
on the existing dungeon generation foundation and introduce gameplay
systems.
