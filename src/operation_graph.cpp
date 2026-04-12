#include "operation_graph.h"

void operation_graph::initialize(int _x, int _y, bool _isabc, bool is_dijkstra) {
    x = _x;
    y = _y;
    isabc = _isabc;
    options = {"Create", "Vertices", "Add Edges", "Kruskal"};
    if (is_dijkstra) {
        options = {"Create", "Vertices", "Add Edges", "Start vertice", "Dijkstra"};
    }

    sub_options = {"Random", "From File"};
}

std::string operation_graph::open_file_path() {
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

void operation_graph::update() {
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
    
    int id = 0;
    for (auto str : options) {
        Rectangle textRect = {x + 20, y + 20 + id * 40, 200, 40 };
        if (str != "Create" && !input.focused && CheckCollisionPointRec(GetMousePosition(), textRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            appear_sub_option = false;
            command = str;
            if (command == "Kruskal" || command == "Dijkstra") {
                is_pending = true;
                break;
            } 
            
            input.appear = true;
            input.initialize(x + 250, y + 20 + id * 40, 200, 40, "Enter value to " + str);
            if (str == "Add Edges") {
                input.initialize(x + 250, y + 20 + id * 40, 200, 40, "Enter edges (u v w)", 1);
            }
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
                    input.initialize(x + sub_id * 250 , y + 100, 200, 40, "nodes and vertices", 1);
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

void operation_graph::draw() {
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