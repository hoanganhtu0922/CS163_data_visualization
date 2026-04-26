#include "infotable.h"
#include <iostream>

void infotable::draw() {
    Rectangle board = { (float)x, (float)y, 150, info.size() * 40};
    DrawRectangleRec(board, Fade(LIGHTGRAY, 0.8f));

    for (size_t i = 0; i < info.size(); i++) {
        Rectangle cell = { (float)x, (float)y + i * 40, 150, 40 };
        DrawTextEx(font, info[i].c_str(), { (float)(x + 10), (float)(y + 10 + i * 40) }, 20, 1, DARKGRAY);
        DrawRectangleLinesEx(cell, 1, GRAY);
        if (CheckCollisionPointRec(GetMousePosition(), cell)) {
            DrawRectangleRec(cell, Fade(PINK, 0.3f));
        }
    }
}

void infotable::add(std::string new_info) {
    info.push_back(new_info);
}

std::string infotable::clicked() {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        std::cout << "Mouse Clicked at: " << GetMousePosition().x << ", " << GetMousePosition().y << std::endl;
        for (int i = 0; i < info.size(); i++) {
            Rectangle cell = { (float)x, (float)y + i * 40, 150, 40 };
            if (CheckCollisionPointRec(GetMousePosition(), cell)) {
                std::cout << "Clicked on: " << info[i] << std::endl;
                return info[i];
            }
        }
    }
    return "none";
}