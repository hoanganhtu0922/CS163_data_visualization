#include "raylib.h"
#include "button.h"
#include <vector>
#include "font.h"
#include <iostream>
#pragma once

// Cấu trúc một Node trong Linked List với các thuộc tính Visualization


int main() {
    // Khởi tạo cửa sổ jskajs
    int screenWidth = 1000;
    int screenHeight = 600;
    
    InitWindow(screenWidth, screenHeight, "Singly Linked List Visualizer - Smooth Transition");
    SetTargetFPS(60);

    font = LoadFont("../../fonts/Roboto-Bold.ttf");
    std::vector <button> buttons;
    buttons.push_back(button::button(50, 50, 150, 50, BLUE, "singly linked list"));

    while (!WindowShouldClose()) {
        BeginDrawing();
        for (auto& b : buttons) {
            b.draw();
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}