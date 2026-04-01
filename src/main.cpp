#include "raylib.h"
#include "button.h"
#include <vector>
#include "font.h"
#include "singly_linked_list.h"
#include "linear_probing.h"
#include <iostream>
#include "trie.h"
#include "avltree.h"
#include "graph_visualization.h"
#include "kruskal.h"
#include "dijsktra.h"
#include "assets.h"
#include "highlight_code.h"

#pragma once

int main() {
    int screenWidth = 1500;
    int screenHeight = 900;
    
    InitWindow(screenWidth, screenHeight, "Data Structure Visualizer");
    SetTargetFPS(60);

    font = LoadFont("../../fonts/Roboto-Bold.ttf");
    std::vector <button> buttons;
    assets::Instance().LoadResources();
    highlight_code::Instance().LoadResources();
    highlight_code::Instance().prebuild_position();

    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
    Vector2 getsz = MeasureTextEx(font, "DSROLL", 100, 1);


    buttons.push_back(button::button(225, 250, 300, 200, {62, 114, 175, 140}, "Singly Linked List"));
    buttons.push_back(button::button(575, 250, 300, 200, {62, 114, 175, 140}, "Hash Table"));
    buttons.push_back(button::button(925, 250, 300, 200, {62, 114, 175, 140}, "Trie"));
    buttons.push_back(button::button(225, 550, 300, 200, {62, 114, 175, 140}, "AVL Tree"));
    buttons.push_back(button::button(575, 550, 300, 200, {62, 114, 175, 140}, "Kruskal"));
    buttons.push_back(button::button(925, 550, 300, 200, {62, 114, 175, 140}, "Dijkstra"));

    std::string selected_option = "none";

    while (!WindowShouldClose()) {
        BeginDrawing();
        DrawRectangleGradientV(0, 0, GetScreenWidth(), GetScreenHeight(), CLITERAL(Color){ 200, 220, 255, 255 }, CLITERAL(Color){ 255, 200, 230, 255 });
        for (auto& b : buttons) {
            b.draw();
        }
        DrawTextEx(font, "DSROLL", {(float)(screenWidth - getsz.x) / 2,  10}, 100, 1, WHITE);
        EndDrawing();

        for (auto& b : buttons) {
            if (b.isClicked()) {
                if (b.text == "Singly Linked List") {
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

                if (b.text == "AVL Tree") {
                    AVLTree avl;
                    selected_option = avl.Run();
                }

                if (b.text == "Kruskal") {
                    kruskal k;
                    k.Run();
                }

                if (b.text == "Dijkstra") {
                    dijkstra d;
                    d.Run();
                }
            }
        }
    }

    CloseWindow();
    return 0;
}