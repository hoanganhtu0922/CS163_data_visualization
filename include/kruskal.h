#include "raylib.h"
#include "graph_visualization.h"
#include "algorithm"
#include "string.h"
#include "duration.h"
#include "camera.h"
#include "menu.h"
#include "operation_graph.h"
#include "iostream"
#include "assets.h"
#include "highlight_code.h" 
#pragma once

struct kruskal{
    graph G;
    std::vector<Edge> mst_edges; // Lưu trữ các cạnh của cây khung nhỏ nhất
    std::vector <std:: vector <Edge>> history; // Lưu trữ lịch sử các bước thực hiện thuật toán
    std::vector <int> snippets; // Lưu trữ các đoạn mã tương ứng với từng bước của thuật toán
    float smoothSpeed = 0.1f;
    float time_per_change = 0.5, progress = 0;
    int curent_state = 0;
    duration progress_duration, speed; // Add this line
    camera cam;
    menu list_data;
    operation_graph opp;
    int vertices = 0, edges = 0;   

    void find_mst();

    void draw();
    void draw_task();

    void update();

    std::string Run();
};