#include "node.h"
#include "raylib.h"
#include <string>
#include <vector>
#include "font.h"
#include "raymath.h"
#include "infotable.h"
#include "pop_up.h"

#pragma once

struct SinglyLinkedList{
    std::string command;
    Node* head = nullptr;
    Node* pop_up_node = nullptr;
    infotable list_info;
    pop_up input;

    const float smoothSpeed = 0.1f;

    void append(int value);
    void delete_node();
    void insert_before();
    void insert_after();
    void update();
    void draw();
    void run();
};