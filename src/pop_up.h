#include "raylib.h"
#include <string.h>
#include "font.h"
#pragma once

struct pop_up {
    int x, y;
    char inputBuffer[16] = "\0"; // Stores the number as a string
    int letterCount = 0;
    bool showPopup = false;
    int value, is_pending = false;

    void update();
    void draw();
};

