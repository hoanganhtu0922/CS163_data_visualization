#include "raylib.h"
#include <string> 
#pragma once

struct Node {
    int data, size = 80;
    bool is_clicking = 0;
    Node* next;
    Vector2 currentPos; 
    Vector2 targetPos;  
    float alpha;
    bool is_hovered();
    bool is_clicked();
};