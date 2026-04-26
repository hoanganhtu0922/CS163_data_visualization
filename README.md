# DSROLL: Data Structure & Algorithm Visualizer

DSROLL is an interactive desktop visualization system designed to help users explore and understand the internal mechanics of core data structures and graph algorithms. Built with C++ and Raylib, the system provides a unified platform for observing algorithm behavior through animated transitions and synchronized pseudocode highlighting.

## Key Features

* Comprehensive Algorithm Suite: Includes Singly Linked List, Hash Table (chaining), Trie, AVL Tree, Kruskal’s MST, and Dijkstra’s Shortest Path.
* Interactive Playback Controller: Navigate through algorithm steps using a timeline scrubber, or adjust the animation rate with a dedicated speed slider.
* Line-based Pseudocode Highlighting: Follow along with the logic via a code panel that highlights the specific line of pseudocode corresponding to the current visual action.
* Dynamic UI & Camera: Supports real-time camera panning and zooming, alongside a force-directed graph layout with draggable nodes.
* Multiple Input Channels: Create datasets via random generation, manual keyboard entry, or by importing data from plain text files.

## Technical Specifications

* Programming Language: C++
* Graphics Library: Raylib
* Build System: CMake (minimum version 3.16)
* Platform: Windows (MSBuild/Visual Studio toolchain)
* Dependencies: Integrated tinyfiledialogs for cross-platform file interaction.

## Project Organization

The codebase follows a modular layer separation:
* include/: Header declarations and interface definitions.
* src/: Module-specific implementations (Algorithm logic and Rendering).
* assets/: Static resources including textures, icons, and fonts.
* raylib/: Internal graphics rendering dependency.

## Development Setup

### Prerequisites
* Windows OS
* CMake and Visual Studio C++ workload installed

### Build Instructions
1. Clone the repository to your local machine.
2. Open the project root directory in your terminal or IDE.
3. Configure CMake:
   cmake -S . -B build
4. Build the project on cmake

## User Manual

1. Launch: Run the application to enter the main menu.
2. Select Module: Choose a data structure or graph algorithm tile.
3. Input Data: Use the operation panel (lower-left) to Create data from a file or keyboard.
4. Control Playback: 
   * Drag the red handle on the progress bar to jump between states.
   * Adjust the speed slider to speed up or slow down the animation.
   * Right-click and drag to pan the camera, and use the scroll wheel to zoom.
