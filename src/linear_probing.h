#include "raylib.h"
#include <vector>
#include <string>
#include <vector>
#include <iostream>
#include "raymath.h"
#include "font.h"
#include "node.h"
#include "camera.h"
#include "operation.h"
#include "duration.h"
#include "menu.h"
#pragma once

struct LinearProbingVisualizer {
    std::vector <std::vector <Node>> list;
    std::vector <std::vector <std::vector <Node>>> history;
    const float smoothSpeed = 0.1f;
    float time_per_change = 0.5, progress = 0;
    int curent_state = 0;
    duration progress_duration, speed;
    camera cam;
    operation opp;
    menu list_data;
    std::string run();
    LinearProbingVisualizer() {
        list.resize(10, std::vector<Node>(1, {-1}));
    };
    void draw();
    void update();
    void insert(int value);
    void delete_node(int value);
    void search(int value);
    void DrawLinkedList(int id);
    void draw_task();
    Vector2 get_pos(int i, int j);
    float get_alpha(int i, int j);
};