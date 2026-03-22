#include "raylib.h"
#include "graph_visualization.h"
#include "algorithm"
#include "string.h"
#include "duration.h"
#include "camera.h"
#include "menu.h"
#include "operation_graph.h"
#include "queue"
#pragma once

struct dijkstra{
    graph G;
    std::vector <graph> history; // Lưu trữ lịch sử các bước thực hiện thuật toán
    float smoothSpeed = 0.1f;
    float time_per_change = 0.5, progress = 0;
    int curent_state = 0;
    duration progress_duration, speed; // Add this line
    camera cam;
    menu list_data;
    operation_graph opp;
    int vertices = 0, edges = 0;   
    int start_node = 0, end_node = 0;

    void find_shortest_path() {
        if (start_node < 0 || start_node >= G.nodes.size() || end_node < 0 || end_node >= G.nodes.size() || start_node == end_node) {
            return; // Invalid start or end node
        }

        history.push_back(G); // Lưu trạng thái ban đầu của đồ thị
        // 1. Sắp xếp các cạnh theo trọng số
        std::vector < std::vector <std::vector <int>>> edge(G.nodes.size());
        int id = 0;
        for (const auto& e : G.edges) {
            edge[e.u_idx].push_back({e.length, e.v_idx, id});
            edge[e.v_idx].push_back({e.length, e.u_idx, id}); // Đồ thị vô hướng
            id++;
        } 

        std::priority_queue <std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;
        std::vector<int> dist(G.nodes.size(), INT_MAX);
        std::vector<int> trace_edge(G.nodes.size(), -1);
        
        dist[start_node] = 0;
        pq.push({0, start_node});

        while (!pq.empty()) {
            int u = pq.top().second;
            int length = pq.top().first;
            pq.pop();

            if (dist[u] < length) continue; // Skip if we have already found a better path

            if (trace_edge[u] != -1) {
                G.edges[trace_edge[u]].is_checking = 1; // Đánh dấu đang kiểm tra cạnh này
                history.push_back(G); // Lưu lại trạng thái hiện tại của đồ thị sau khi đánh dấu cạnh đang kiểm tra
            }

            G.nodes[u].is_checking = 1; // Đánh dấu đang kiểm tra node này
            history.push_back(G); // Lưu lại trạng thái hiện tại của đồ thị sau khi đánh dấu node đang kiểm tra

            for (const auto& neighbor : edge[u]) {
                int weight = neighbor[0];
                int v = neighbor[1];
                G.edges[neighbor[2]].is_focused = 1; // Đánh dấu đang kiểm tra cạnh này
                history.push_back(G); // Lưu lại trạng thái hiện tại của đồ thị sau khi đánh dấu cạnh đang kiểm tra
                
                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    pq.push({dist[v], v});
                    trace_edge[v] = neighbor[2]; // Lưu lại cạnh đã được sử dụng để đi đến node v
                } else {
                    G.edges[neighbor[2]].is_focused = 0; // Bỏ đánh dấu tập trung sau khi kiểm tra cạnh này
                }
            }

            G.nodes[u].is_checking = 0; // Bỏ đánh dấu kiểm tra sau khi đã kiểm tra xong node này
            history.push_back(G); // Lưu lại trạng thái hiện tại của đồ thị sau khi bỏ đánh dấu node đang kiểm tra
        }

        for (auto &edge : G.edges) {
            edge.is_focused = 0; // Đảm bảo tất cả các cạnh ban đầu không được đánh dấu tập trung
        }

        history.push_back(G); // Lưu lại trạng thái cuối cùng của đồ thị sau khi hoàn thành thuật toán
        int cur = end_node;
        while (trace_edge[cur] != -1) {
            G.edges[trace_edge[cur]].is_focused = 1; // Đánh dấu các cạnh trên đường đi ngắn nhất
            cur = (G.edges[trace_edge[cur]].u_idx == cur) ? G.edges[trace_edge[cur]].v_idx : G.edges[trace_edge[cur]].u_idx; // Đi ngược lại theo đường đi đã tìm được
            G.nodes[cur].is_focused = 1; // Đánh dấu các node trên đường đi ngắn nhất
            history.push_back(G); //
        }

        for (auto &edge : G.edges) {
            edge.is_focused = 0;
        }

        history.push_back(G); // Lưu lại trạng thái cuối cùng của đồ thị sau khi đánh dấu đường đi ngắn nhất
        G = history[0]; // Quay lại trạng thái ban đầu của đồ thị để có thể chạy lại thuật toán nếu muốn
    }

    void draw() {
        list_data.update();
        speed.update();
        opp.update();

        if (history.size()) {
            double tot_time = time_per_change * (history.size() - 1);
            double elapsed_time = progress + curent_state * time_per_change;
            progress_duration.value = (int)((elapsed_time / tot_time) * 200);
            progress_duration.update();
        }

        if (progress_duration.is_changed) {
            float tot_time = time_per_change * (history.size() - 1);
            curent_state = (int)((progress_duration.value / 200.0f) * (history.size() - 1));
            float start_bar = (curent_state * time_per_change) / tot_time * 200;
            float end_bar = ((curent_state + 1) * time_per_change) / tot_time * 200;
            progress = ((progress_duration.value - start_bar) / (end_bar - start_bar)) * time_per_change;
            if (progress < 0) progress = 0;
            progress_duration.is_changed = false;
            G = history[curent_state];
        }

        if (speed.is_changed) {
            time_per_change = 0.05f + (100 - speed.value) / 50;
            speed.is_changed = false;
        }
        G.draw();
    }

    void draw_task() {
        progress_duration.draw();
        speed.draw();
        list_data.draw();
        opp.draw();
        G.UpdateGraphLayout();
    }

    void update() {
        if (opp.is_pending) {
            history.clear();
            curent_state = 0; progress = 0;
            progress_duration.value = 0;
            if (opp.command == "Vertices") {
                vertices = opp.value;
                G.gen_random(vertices, edges);
            } else if (opp.command == "Edges") {
                edges = opp.value;
                G.gen_random(vertices, edges);
            } else if (opp.command == "Intit") {
                vertices = GetRandomValue(10, 20);
                edges = GetRandomValue(10,vertices * (vertices - 1) / 3);
                G.gen_random(vertices, edges);
            } else if (opp.command == "Dijkstra") {
                find_shortest_path();
            } else if (opp.command == "start") {
                if (start_node >= 0 && start_node < G.nodes.size()) {
                    G.nodes[start_node].is_focused = false; // Remove highlight after recording state
                }
                start_node = opp.value;
                if (start_node >= 0 && start_node < G.nodes.size()) {
                    G.nodes[start_node].is_focused = true; // Highlight new start node
                }
            } 
            opp.is_pending = false;
        }
    }

    std::string Run() {
        opp.initialize(20, 600, 0, 1);
        cam.initialize();
        speed.initialize(850, 800, 100, 50);
        list_data.init(20, 50);
        list_data.choose = "Dijkstra";
        list_data.focused = 0;
        int screenWidth = 1000;
        int screenHeight = 600; 

        progress_duration.initialize(550, 800, 200, 0);

        while (!WindowShouldClose()) {
            if (list_data.choose != "Dijkstra") {
                return list_data.choose;
            }
            
            cam.update_zoom();
            G.cam = cam; // Cập nhật camera cho graph
            if (progress_duration.isDraggingHandle == 0 && speed.isDraggingHandle == 0 && G.is_dragging() == 0) {
                cam.update_dragging();
                G.cam = cam; // Cập nhật camera cho graph sau khi có thể đã thay đổi do kéo node
            }

            if (curent_state + 1 < history.size()) {
                progress += GetFrameTime();
                if (progress > time_per_change) {
                    progress = 0;
                    G = history[++curent_state];
                }
            } else {
                progress = 0;
            }

            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawRectangleGradientV(0, 0, GetScreenWidth(), GetScreenHeight(), CLITERAL(Color){ 200, 220, 255, 255 }, CLITERAL(Color){ 255, 200, 230, 255 });

            BeginMode2D(cam.camera);
            draw();
            EndMode2D();
            if (curent_state >= (int)history.size() - 1) {
                update();
            }
            //update();
            draw_task();
            EndDrawing();
        }
        CloseWindow();
        return "";
    }
};