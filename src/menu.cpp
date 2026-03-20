#include "menu.h"
#include "assert.h"
void menu::init(int _x, int _y) {
    x = _x, y = _y;
    option.push_back("Linked List");
    option.push_back("Hash Table");
    option.push_back("AVL Tree");
    option.push_back("Trie");
    option.push_back("Dijkstra");
    option.push_back("Kruskal");
}

void menu::draw() {
    Color background = {189, 178, 255, 255};
    Rectangle textRect = {x, y, 200, 40 };
    float opacity = 0.8f;
    if (CheckCollisionPointRec(GetMousePosition(), textRect)) {
        opacity = 1.0f;
    }
    DrawRectangleRec(textRect, Fade(background, opacity));
    DrawRectangleLinesEx(textRect, 1, DARKGRAY);
    Vector2 textSize = MeasureTextEx(font, choose.c_str(), 20, 1);
    DrawTextEx(font, choose.c_str(), { textRect.x + (textRect.width - textSize.x) / 2, textRect.y + (textRect.height - textSize.y) / 2 }, 20, 1, WHITE);

    if (focused) {
        int id = 1;
        for (auto str : option) {
            if (str == choose)
                continue;
            Color background = {189, 178, 255, 255};
            Rectangle textRect = {x, y + id * 40, 200, 40 };
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

    if (!focused) {
        DrawTriangle({(float)x + 200 - 20, (float)y + 15}, {(float)x + 200 - 15, (float)y + 25}, {(float)x + 200 - 10, (float)y + 15}, WHITE);
    } else {
        DrawTriangle({ (float)x + 180, (float)y + 25 }, { (float)x + 190, (float)y + 25 }, { (float)x + 185, (float)y + 15 }, WHITE);
    }
}

void menu::update() {
    Rectangle textRect = {x, y, 200, 40 };
    if (focused == 0) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mouse = GetMousePosition();
            if (CheckCollisionPointRec(mouse, textRect)) {
                focused = 1;
            }
        }
    } else {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mouse = GetMousePosition();
            if (CheckCollisionPointRec(mouse, textRect)) {
                focused ^= 1;
            }
        }
        int id = 1;
        for (auto str : option) {
            if (str == choose)
                continue;
            Color background = {61,112,171};
            Rectangle textRect = {x, y + id * 40, 200, 40 };
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                Vector2 mouse = GetMousePosition();
                if (CheckCollisionPointRec(mouse, textRect)) {
                    choose = str;
                    focused = 0;
                }
            }
            id++;
        }
    }
}