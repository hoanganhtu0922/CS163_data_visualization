#include "operation.h"

void operation::initialize(int _x, int _y, bool _isabc) {
    x = _x;
    y = _y;
    isabc = _isabc;
    options = {"Insert", "Delete", "Search"};
}

void operation::update() {
    if (input.is_pending) {
        if (isabc == false) {
            value = input.value;
            input.is_pending = false;
            is_pending = true;
        } else {
            str_value = input.num;
            input.is_pending = false;
            is_pending = true;
        }
    }

    input.isabc = isabc;

    int id = 0;
    for (auto str : options) {
        Rectangle textRect = {x + 20, y + 20 + id * 40, 200, 40 };
        if (!input.focused && CheckCollisionPointRec(GetMousePosition(), textRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            command = str;
            input.appear = true;
            input.initialize(x + 250, y + 20 + id * 40, 200, 40, "Enter value to " + str);
            break;
        }
        id++;
    }
}

void operation::draw() {
    input.update();
    input.draw(); 
    int id = 0;
    for (auto str : options) {
        Color background = {61,112,171};
        Rectangle textRect = {x + 20, y + 20 + id * 40, 200, 40 };
        float opacity = 0.8f;
        if (CheckCollisionPointRec(GetMousePosition(), textRect)) {
            opacity = 1.0f;
        }
        DrawRectangleRec(textRect, Fade(background, opacity));
        DrawRectangleLinesEx(textRect, 1, DARKGRAY);
        Vector2 textSize = MeasureTextEx(font, str.c_str(), 20, 1);
        DrawTextEx(font, str.c_str(), { textRect.x + (textRect.width - textSize.x) / 2, textRect.y + (textRect.height - textSize.y) / 2 }, 20, 1, WHITE);
        id++;
    }
}
