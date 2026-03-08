#include "linear_probing.h"

LinearProbingVisualizer::LinearProbingVisualizer() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        table[i].targetPos = { (float)(100 + i * 90), 300 };
        table[i].currentPos = { (float)(100 + i * 90), 100 }; 
    }
}

int LinearProbingVisualizer::hashFunction(int key) {
    return key % TABLE_SIZE;
}

void LinearProbingVisualizer::insert() {
    if (!is_inserting) return;

    if (table[carrying_index].occupied) {
        carrying_index = (carrying_index + 1) % TABLE_SIZE;
    } else {
        is_inserting = false;
        table[carrying_index].value = carrying_value;
        table[carrying_index].occupied = true;
        table[carrying_index].alpha = 0.0f;
    }
}

void LinearProbingVisualizer::update() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        table[i].currentPos = Vector2Lerp(table[i].currentPos, table[i].targetPos, smoothSpeed);
        
        if (table[i].occupied && table[i].alpha < 1.0f) {
            table[i].alpha += smoothSpeed;
        }
    }
}

void LinearProbingVisualizer::draw() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Rectangle rect = { table[i].currentPos.x - 40, table[i].currentPos.y - 40, 80, 80 };
        DrawRectangleRec(rect, Fade(DARKGRAY, 0.3f));

        DrawText(TextFormat("[%d]", i), (int)rect.x + 30, (int)rect.y + 85, 15, GRAY);

        if (table[i].occupied) {
            Color textColor = Fade(MAROON, table[i].alpha);
            const char* text = TextFormat("%d", table[i].value);
            
            int fontSize = 20;
            int textWidth = MeasureText(text, fontSize);
            
            int posX = (int)table[i].currentPos.x - textWidth / 2;
            int posY = (int)table[i].currentPos.y - 10;
            
            DrawText(text, posX, posY, fontSize, textColor);
        }

        if (is_inserting && i == carrying_index) {
            DrawRectangleLinesEx(rect, 2, RED);
        } else {
            DrawRectangleLinesEx(rect, 2, GRAY);
        }
    }
}

void LinearProbingVisualizer::run() {
    //InitWindow(1000, 600, "Linear Probing Visualizer");
    SetTargetFPS(60);

    double lst_time = GetTime();
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_A) && !is_inserting) {
            is_inserting = true;
            carrying_value = GetRandomValue(0, 999);
            carrying_index = hashFunction(carrying_value);
            lst_time = GetTime();
        }

        double current_time = GetTime();
        double delta_time = current_time - lst_time;
        if (delta_time > 0.5) {
            insert();
            lst_time = current_time;
        }

        update();

        BeginDrawing();
        ClearBackground(RAYWHITE);
        draw();
        EndDrawing();
    }
    CloseWindow();
}