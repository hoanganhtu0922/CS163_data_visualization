#include <raylib.h>
#include <string>
#include "font.h"
#pragma once

struct text_box {
    Rectangle bounds;
    std::string text, num;
    bool isabc = false;
    bool appear = false;
    bool is_pending = false;
    int value;
    bool focused = false;
    double cursorTimer = 0.0;
    bool cursorVisible = true;
    void update();
    void draw();
    void initialize(int x, int y, int width, int height, std::string initialText);
};