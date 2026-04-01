#include "textbox.h"

void text_box::initialize(int x, int y, int width, int height, std::string initialText) {
    bounds = { (float)x, (float)y, (float)width, (float)height };
    text = initialText;
}

void text_box::draw() {
    if (!appear) return;   
    Vector2 textSize = MeasureTextEx(font, text.c_str(), 20, 1);
    DrawTextEx(font, text.c_str(), { bounds.x + bounds.width/2 - textSize.x/2, bounds.y - 30 }, 20, 1, DARKGRAY);
    DrawRectangleRec(bounds, LIGHTGRAY);
    assets::Instance().draw_texture("random_icon", { bounds.x + 1 + bounds.width, bounds.y}, 0.14f);

    if (focused)
        DrawRectangleLinesEx(bounds, 2, BLUE);
    else
        DrawRectangleLinesEx(bounds, 1, DARKGRAY);

    DrawTextEx(font, num.c_str(), { bounds.x + 10, bounds.y + 10 }, 20, 1, BLACK);

    if (focused && cursorVisible) {
        Vector2 numSize = MeasureTextEx(font, num.c_str(), 20, 1);
        DrawTextEx(font, "|", { bounds.x + 10 + numSize.x, bounds.y + 10 }, 20, 1, BLACK);
    }
}

void text_box::update() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        focused = CheckCollisionPointRec(GetMousePosition(), bounds);
    }

    if (assets::Instance().is_clicked({ bounds.x + 1 + bounds.width, bounds.y}, "random_icon", 0.14f)) {
        if (isabc) {
            int len = GetRandomValue(5, 15);
            num = "";
            for (int i = 0; i < len; i++) {
                char randomChar = 'a' + GetRandomValue(0, 25);
                num.push_back(randomChar);
            }
        } else {
            num = std::to_string(GetRandomValue(1, 999));
        }
    }

    if (!focused) return;

    char key = GetCharPressed();

    while (key > 0) {
        if (key >= '0' && key <= '9' && num.length() < 3 && isabc == false) {
            num.push_back((char)key);
        }

        if (key >= 'a' && key <= 'z' && num.length() < 20 && isabc == true) {
            num.push_back((char)key);
        }

        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE) && !num.empty()) {
        num.pop_back();
    }

    cursorTimer += GetFrameTime();

    if (cursorTimer >= 0.5f) {
        cursorTimer = 0;
        cursorVisible = !cursorVisible;
    }


    if (IsKeyPressed(KEY_ENTER) && !num.empty()) {
        str_value = num;
        appear = false;
        is_pending = true;
    }
}


