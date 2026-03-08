#include "raylib.h"
#include "button.h"
#include <vector>
#include "font.h"
#include "singly_linked_list.h"
#include "linear_probing.h"
#include <iostream>
#pragma once


int main() {
    int screenWidth = 1000;
    int screenHeight = 600;
    
    InitWindow(screenWidth, screenHeight, "Singly Linked List Visualizer - Smooth Transition");
    SetTargetFPS(60);

    font = LoadFont("../../fonts/Roboto-Bold.ttf");
    std::vector <button> buttons;

    buttons.push_back(button::button(50, 50, 150, 50, BLUE, "singly linked list"));
    buttons.push_back(button::button(250, 50, 150, 50, GREEN, "linear probing"));


    while (!WindowShouldClose()) {
        BeginDrawing();
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