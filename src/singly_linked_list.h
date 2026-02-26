#include "node.h"
#include "raylib.h"
#include <string>
#include <vector>
#include "font.h"
#include "raymath.h"

#pragma once

struct SinglyLinkedList{
    Node* head = nullptr;
    const float smoothSpeed = 0.1f;

    void append(int value);
    void update();
    void draw();
};