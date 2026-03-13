#include "singly_linked_list.h"
#include <iostream>

void SinglyLinkedList::append(int value) {
    Node* newNode = new Node{value, 80, 0, nullptr, {0, 0}, {0, 0}, 0.0f};
    
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

void SinglyLinkedList::insert_before() {
    Node *cur = head;
    Node *prev = nullptr;

    while (cur != nullptr) {
        if (cur == pop_up_node) {
            Node* newNode = new Node{input.value, 80, 0, nullptr, {0, 0}, {0, 0}, 0.0f};
            if (cur == head) {
                newNode->next = head;
                head = newNode;
            } else {
                prev->next = newNode;
                newNode->next = cur;
            }

            newNode->targetPos = { cur->targetPos.x - 160, cur->targetPos.y };
            newNode->currentPos = { newNode->targetPos.x - 100, newNode->targetPos.y };
            cur = newNode;
            while (cur) {
                cur->targetPos.x += 160;
                cur = cur->next;
            }
            pop_up_node = nullptr;
            return;
        }
        prev = cur;
        cur = cur->next;
    }
}

void SinglyLinkedList::insert_after() {

}

void SinglyLinkedList::delete_node() {
    Node* cur = head;
    while (cur != nullptr) {
        if (cur == pop_up_node) {
            if (cur == head) {
                head = head->next;
            } else {
                Node* temp = head;
                while (temp->next != cur) {
                    temp = temp->next;
                }
                temp->next = cur->next;
            }
            delete cur;
            pop_up_node = nullptr;
            return;
        }
        cur = cur->next;
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
        
        DrawCircleV(center, temp->size / 2, mainColor);
        
        DrawCircleLinesV(center, temp->size / 2, borderColor);

        std::string text = std::to_string(temp->data);
        int textWidth = MeasureText(text.c_str(), 20);
        DrawText(text.c_str(), (int)temp->currentPos.x - textWidth / 2, (int)temp->currentPos.y - 10, 20, textColor);

        temp = temp->next;
    }

    input.update();
    input.draw();
    Node *cur = head;

    while (cur) {
        if (cur->is_clicking) {
            list_info.x = cur->targetPos.x + 40;
            list_info.y = cur->targetPos.y - 50;
            list_info.draw();
            std::string clicked_info = list_info.clicked();
            if (clicked_info != "none") {
                input.showPopup = 1;
                cur -> is_clicking = false;
                input.x = cur->targetPos.x + 40;
                input.y = cur->targetPos.y - 60;
                pop_up_node = cur;
                command = clicked_info;
            } 
        }
        cur = cur->next;
    }

    if (input.is_pending) {
        if (input.value != 0) {
            if (command == "delete") {
                delete_node();
            } else if (command == "insert before") {
                insert_before();
            } else if (command == "insert after") {
                insert_after();
            }
        }
        input.is_pending = false;
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Node *cur = head;
        while (cur) {
            if (cur->is_hovered() && !input.showPopup) {
                cur->is_clicking = true;
            } else {
                cur->is_clicking = false;
            }
            cur = cur->next;
        }
    }
}

void SinglyLinkedList::run() {
    int screenWidth = 1000;
    int screenHeight = 600;
    list_info.add("insert before");
    list_info.add("delete");
    list_info.add("insert after");

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
