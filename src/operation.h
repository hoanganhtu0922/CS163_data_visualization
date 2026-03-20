#include "raylib.h"
#include <vector>
#include <string>
#include "textbox.h"
#include "font.h"
#include "textbox.h"
#pragma once

struct operation {
    int x, y;
    std::vector <std::string> options;
    text_box input;
    std::string command;
    int value = 0, pop_up_app = 0, is_pending = false;
    std::string str_value;
    bool isabc = false;
    void initialize(int _x, int _y, bool _isabc = false);
    void update();
    void draw();
};