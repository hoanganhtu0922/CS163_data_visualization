#include "raylib.h"
#include "button.h"
#include <vector>
#include "font.h"
#include "singly_linked_list.h"
#include "linear_probing.h"
#include <iostream>
#pragma once


int main() {
    int screenWidth = 1500;
    int screenHeight = 900;
    
    InitWindow(screenWidth, screenHeight, "Data Structure Visualizer");
    SetTargetFPS(60);

    font = LoadFont("../../fonts/Roboto-Bold.ttf");
    std::vector <button> buttons;

    buttons.push_back(button::button(50, 50, 150, 50, GREEN, "singly linked list"));
    buttons.push_back(button::button(250, 50, 150, 50, GREEN, "linear probing"));


    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(Fade(LIGHTGRAY, 0.4f));
        for (auto& b : buttons) {
            b.draw();
        }
        EndDrawing();

        for (auto& b : buttons) {
            if (b.isClicked()) {
                if (b.text == "singly linked list") {
                    SinglyLinkedList list;
                    list.run();
                }

                if (b.text == "linear probing") {
                    LinearProbingVisualizer visualizer;
                    visualizer.run();
                }
            }
        }
    }

    CloseWindow();
    return 0;
}