#include "raylib.h"
#include "assets.h"
#pragma once 
struct duration {
    int x, y, length, value;
    bool is_changed = 0;
    bool is_paused = 0;
    void draw();
    void update();
    Color progressBarColor;
    Color hoverProgressBarColor;
    Color backgroundColor;
    Color hoverBackgroundColor;
    Color handleColor;
    Color hoverHandleColor;
    float barHeight;
    float handleRadius;
    bool isHoveringBar;
    bool isHoveringHandle;
    bool isDraggingHandle;
    void initialize(int x, int y, int length, int value);
};
