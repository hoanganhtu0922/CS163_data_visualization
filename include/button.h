#include "raylib.h"
#include "raymath.h"
#include "font.h"
#include <string>
#include <vector>
#pragma once

struct button{
    int x, y, width, height;
    Color color;
    std::string text;

    button(int x, int y, int width, int height, Color color, std::string text);

    void draw();

    bool isClicked();

    bool isHovered();
};