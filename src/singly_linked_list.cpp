#include "singly_linked_list.h"

void SinglyLinkedList::append(int value) {
    Node* newNode = new Node{value, nullptr, {0, 0}, {0, 0}, 0.0f};
    
    if (!head) {
        head = newNode;
        newNode->targetPos = { 100, 300 };
        newNode->currentPos = { 0, 300 }; 
    } else {
        Node* temp = head;
        int count = 1;
        while (temp->next) {
            temp = temp->next;
            count++;
        }
        temp->next = newNode;
        newNode->targetPos = { (float)(100 + count * 160), 300 };
        newNode->currentPos = { newNode->targetPos.x - 100, 300 }; 
    }
}

void SinglyLinkedList::update() {
    Node* temp = head;
    while (temp) {
        temp->currentPos = Vector2Lerp(temp->currentPos, temp->targetPos, smoothSpeed);
        if (temp->alpha < 1.0f) temp->alpha += smoothSpeed;
        
        temp = temp->next;
    }
}

void SinglyLinkedList::draw() {
    Node* temp = head;
    while (temp) {
        Color mainColor = Fade(SKYBLUE, temp->alpha);
        Color borderColor = Fade(BLUE, temp->alpha);
        Color textColor = Fade(DARKGRAY, temp->alpha);
        Color arrowColor = Fade(GRAY, temp->alpha);

        if (temp->next) {
            Vector2 start = { temp->currentPos.x + 40, temp->currentPos.y };
            Vector2 end = { temp->next->currentPos.x - 40, temp->next->currentPos.y };
            DrawLineEx(start, end, 2.0f, arrowColor);
            DrawTriangle({end.x - 10, end.y - 5}, {end.x - 10, end.y + 5}, end, arrowColor);
        }

        Vector2 center = { temp->currentPos.x, temp->currentPos.y};
        
        DrawCircleV(center, 35, mainColor);
        
        DrawCircleLinesV(center, 35, borderColor);

        std::string text = std::to_string(temp->data);
        int textWidth = MeasureText(text.c_str(), 20);
        DrawText(text.c_str(), (int)temp->currentPos.x - textWidth / 2, (int)temp->currentPos.y - 10, 20, textColor);

        temp = temp->next;
    }
}

void SinglyLinkedList::run() {
    int screenWidth = 1000;
    int screenHeight = 600;
    
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        update();
        draw();
        if (IsKeyPressed(KEY_A)) {
            int value = rand() % 100;
            append(value);
        }
        EndDrawing();
    }
}