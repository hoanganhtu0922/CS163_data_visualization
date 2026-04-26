#include "raylib.h"
#pragma once

struct camera {
    Camera2D camera;
    Vector2 lastMouse;
    int screenWidth = 1000, screenHeight = 600;
    void initialize();
    void update_dragging();
    void update_zoom();
};