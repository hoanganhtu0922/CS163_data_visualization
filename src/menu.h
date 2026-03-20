#include "raylib.h"
#include <vector>
#include <string> 
#include "font.h"
#pragma once

struct menu{
    int focused = 0;
    int x, y;
    std::vector <std::string> option;
    std::string choose;
    void init(int _x, int _y);
    void draw();
    void update();
};