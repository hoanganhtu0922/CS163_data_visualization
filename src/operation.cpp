#include "operation.h"

void operation::initialize(int _x, int _y, bool _isabc) {
    x = _x;
    y = _y - 40;
    isabc = _isabc;
    options = {"Create", "Update", "Insert", "Delete", "Search"};
    sub_options = {"Random", "KeyBoard", "From File"};
    sub_option_boxes.resize(sub_options.size() - 1);
    for (auto &x : sub_option_boxes) {
        x.initialize(0, 0, 200, 40, "");
    }
}

std::string operation::open_file_path() {
    const char* filters[] = { "*.txt" };
    const char* file_path = tinyfd_openFileDialog("Select a text file", "", 1, filters, "Text files", 0);
    std::string path;
    if (file_path) {
        for (int i = 0; file_path[i] != '\0'; ++i) {
            path += file_path[i];
        }
    }
    return path;
}

void operation::update() {
    if (input.is_pending) {
        if (isabc == false) {
            str_value = input.str_value;
            input.is_pending = false;
            is_pending = true;
        } else {
            str_value = input.str_value;
            input.is_pending = false;
            is_pending = true;
        }

        appear_sub_option = false;
    }

    input.isabc = isabc;

    int id = 0;
    for (auto str : options) {
        Rectangle textRect = {x + 20, y + 20 + id * 40, 200, 40 };
        if (str != "Create" && !input.focused && CheckCollisionPointRec(GetMousePosition(), textRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            appear_sub_option = false;
            command = str;
            input.appear = true;
            input.initialize(x + 250, y + 20 + id * 40, 200, 40, "Enter value to " + str);
            break;
        }

        if (str == "Create" && !input.focused && CheckCollisionPointRec(GetMousePosition(), textRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            //command = str;
            appear_sub_option = true;
            input.appear = false;
            break;
        }
        id++;
    }

    if (appear_sub_option) {
        int sub_id = 1;
        for (auto str : sub_options) {
            Rectangle textRect = {x + sub_id * 250, y + 20, 200, 40 };
            if (!input.focused && CheckCollisionPointRec(GetMousePosition(), textRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                command = str;
                if (str == "Random") {
                    input.appear = true;
                    input.initialize(x + sub_id * 250 , y + 100, 200, 40, "Enter number of nodes");
                } else if (str == "KeyBoard") {
                    input.appear = true;
                    input.initialize(x + sub_id * 250, y + 100, 200, 40, "Enter all values (split by space)");
                } else if (str == "From File") {
                    str_value = open_file_path();
                    is_pending = true;
                }
                break;
            }
            sub_id++;
        }
    }
}

void operation::draw() {
    input.update();
    input.draw(); 
    int id = 0;
    for (auto str : options) {
        Color background = {62, 114, 175, 140};
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

    if (appear_sub_option) {
        int sub_id = 1;
        for (auto str : sub_options) {
            Color background = {62, 114, 175, 140};
            Rectangle textRect = {x + sub_id * 250, y + 20, 200, 40 };
            float opacity = 0.8f;
            if (CheckCollisionPointRec(GetMousePosition(), textRect)) {
                opacity = 1.0f;
            }
            DrawRectangleRec(textRect, Fade(background, opacity));
            DrawRectangleLinesEx(textRect, 1, DARKGRAY);
            Vector2 textSize = MeasureTextEx(font, str.c_str(), 20, 1);
            DrawTextEx(font, str.c_str(), { textRect.x + (textRect.width - textSize.x) / 2, textRect.y + (textRect.height - textSize.y) / 2 }, 20, 1, WHITE);
            sub_id++;
        }
    }
}
