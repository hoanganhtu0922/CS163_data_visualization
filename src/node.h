#include "raylib.h"
#include <string> 
#pragma once

struct Node {
    int data, size = 80;
    bool is_clicking = 0, is_focused = 0;
    Vector2 start_pos, current_pos, target_pos; 
    float alpha_start = 0.0f, alpha_end = 1.0f;
    bool is_checking = false;
};