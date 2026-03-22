#include "raylib.h"
#include <vector>
#include "raymath.h"
#include "font.h"
#include <set>
#include "camera.h"
#include <string>
#pragma once

struct Node_graph {
    Vector2 pos;
    Vector2 force;
    Vector2 lastMouse;
    int id = 0;
    bool is_checking = 0, is_focused = 0;
    bool update_dragging(camera &cam) {
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && Vector2Distance(GetScreenToWorld2D(GetMousePosition(), cam.camera), pos) < 40.0f) {
            force = {0, 0};
            Vector2 currentMouse = GetScreenToWorld2D(GetMousePosition(), cam.camera);
            pos = currentMouse; // Move node to follow mouse

            lastMouse = currentMouse; 
            return true;
        }

        return false;
    }
};

struct Edge {
    int u_idx, v_idx, length; // Indices of connected nodes and edge length
    bool is_checking = 0, is_focused = 0; // Trạng thái kiểm tra và tập trung cho thuật toán
};

struct graph {
    std::vector<Node_graph> nodes;
    std::vector<Edge> edges;
    float c1 = 1.0f; // Attractive force constant
    float c2 = 200.0f; // Desired edge length
    float c3 = 10000.0f; // Repulsive force constant
    float c4 = 2.0f; // Movement speed factor
    camera cam;

    bool is_dragging();
    void UpdateGraphLayout();
    void draw();
    void gen_random(int n, int m);
};

