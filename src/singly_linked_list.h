#include "node.h"
#include "raylib.h"
#include <string>
#include <vector>
#include "font.h"
#include "raymath.h"
#include "infotable.h"
#include "pop_up.h"
#include "textbox.h"
#include <vector>
#include "duration.h"
#include "camera.h"
#include "operation.h"
#include "menu.h"

#pragma once

struct SinglyLinkedList{
    std::vector <Node> list;
    std::vector <std::vector <Node>> history;
    const float smoothSpeed = 0.1f;
    float time_per_change = 0.5, progress = 0;
    int curent_state = 0;
    duration progress_duration, speed; // Add this line
    camera cam;
    operation opp;
    menu list_data;
    void insert(int value);
    void delete_node(int value);
    void draw();
    std::string run();
    void draw_task();
    void search(int value);
    void update();
    Vector2 get_pos(int i);
    float get_alpha(int i);
};