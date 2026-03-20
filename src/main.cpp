#include "raylib.h"
#include "button.h"
#include <vector>
#include "font.h"
#include "singly_linked_list.h"
#include "linear_probing.h"
#include <iostream>
#include "trie.h"

#pragma once

int main() {
    int screenWidth = 1500;
    int screenHeight = 900;
    
    InitWindow(screenWidth, screenHeight, "Data Structure Visualizer");
    SetTargetFPS(60);

    font = LoadFont("../../fonts/Roboto-Bold.ttf");
    std::vector <button> buttons;

    buttons.push_back(button::button(50, 50, 150, 50, GREEN, "singly linked list"));
    buttons.push_back(button::button(250, 50, 150, 50, GREEN, "Hash Table"));
    buttons.push_back(button::button(450, 50, 150, 50, GREEN, "Trie"));

    std::string selected_option = "none";

    while (!WindowShouldClose()) {
        if (selected_option == "singly linked list") {
            SinglyLinkedList list;
            selected_option = list.run();
            continue;
        }

        if (selected_option == "Hash Table") {
            LinearProbingVisualizer linear;
            selected_option = linear.run();
            continue;
        }

        if (selected_option == "Trie") {
            Trie heh;
            heh.Run();
            continue;
        }

        BeginDrawing();
        ClearBackground(Fade(LIGHTGRAY, 0.4f));
        DrawRectangleGradientV(0, 0, GetScreenWidth(), GetScreenHeight(), CLITERAL(Color){ 200, 220, 255, 255 }, CLITERAL(Color){ 255, 200, 230, 255 });
        for (auto& b : buttons) {
            b.draw();
        }
        EndDrawing();

        for (auto& b : buttons) {
            if (b.isClicked()) {
                if (b.text == "singly linked list") {
                    SinglyLinkedList list;
                    selected_option = list.run();
                }

                if (b.text == "Hash Table") {
                    LinearProbingVisualizer linear;
                    selected_option = linear.run();
                }

                if (b.text == "Trie") {
                    Trie heh;
                    heh.Run();
                }
            }
        }
    }

    CloseWindow();
    return 0;
}