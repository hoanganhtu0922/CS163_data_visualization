#include "raylib.h"
#include "graph_visualization.h"
#include "algorithm"
#include "string.h"
#include "duration.h"
#include "camera.h"
#include "menu.h"
#include "operation_graph.h"
#include "queue"
#include "assets.h"
#include "highlight_code.h" 
#pragma once

struct snapshot_save{
    std::vector <int> vertice, edge;
    int checking_vertice = -1, checking_edge_a = -1, checking_edge_b = -1;
};

struct dijkstra{
    graph G;
    std::vector <snapshot_save> history;
    std::vector <int> snippets; // Lưu trữ các đoạn mã tương ứng với từng bước trong thuật toán để hiển thị
    float smoothSpeed = 0.1f;
    float time_per_change = 0.5, progress = 0;
    int curent_state = 0;
    duration progress_duration, speed; // Add this line
    camera cam;
    operation_graph opp;
    int vertices = 0, edges = 0;   
    int start_node = 0;

    void find_shortest_path();

    void set_stage();
    void draw();
    void draw_task();

    void update();

    std::string Run();
};