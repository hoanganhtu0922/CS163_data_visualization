#include "raylib.h"
#include "graph_visualization.h"
#include "algorithm"
#include "string.h"
#include "duration.h"
#include "camera.h"
#include "menu.h"
#include "operation_graph.h"

#pragma once

struct kruskal{
    graph G;
    std::vector<Edge> mst_edges; // Lưu trữ các cạnh của cây khung nhỏ nhất
    std::vector <std:: vector <Edge>> history; // Lưu trữ lịch sử các bước thực hiện thuật toán
    float smoothSpeed = 0.1f;
    float time_per_change = 0.5, progress = 0;
    int curent_state = 0;
    duration progress_duration, speed; // Add this line
    camera cam;
    menu list_data;
    operation_graph opp;
    int vertices = 0, edges = 0;   

    void find_mst() {
        // 1. Sắp xếp các cạnh theo trọng số
        curent_state = 0; progress = 0;
        std::vector<Edge> sorted_edges = G.edges;
        std::sort(sorted_edges.begin(), sorted_edges.end(), [](const Edge& a, const Edge& b) {
            return a.length < b.length;
        });

        for (auto &edge : sorted_edges) {
            edge.is_checking = 0; // Đảm bảo tất cả các cạnh ban đầu không được đánh dấu kiểm tra
            edge.is_focused = 0; // Đảm bảo tất cả các cạnh ban đầu không được đánh dấu tập trung
        }

        history.push_back(sorted_edges);

        // 2. Khởi tạo cấu trúc Union-Find để kiểm tra chu trình
        int n = G.nodes.size();
        std::vector<int> parent(n), rank(n, 0);
        for (int i = 0; i < n; i++) parent[i] = i;

        auto find = [&](int u) {
            while (u != parent[u]) {
                parent[u] = parent[parent[u]]; // Path compression
                u = parent[u];
            }
            return u;
        };

        auto unite = [&](int u, int v) {
            int root_u = find(u);
            int root_v = find(v);
            if (root_u != root_v) {
                if (rank[root_u] > rank[root_v]) {
                    parent[root_v] = root_u;
                } else if (rank[root_u] < rank[root_v]) {
                    parent[root_u] = root_v;
                } else {
                    parent[root_v] = root_u;
                    rank[root_u]++;
                }
                return true;
            }
            return false;
        };

        // 3. Duyệt qua các cạnh đã sắp xếp và thêm vào MST nếu không tạo thành chu trình
        for (auto it_edge = sorted_edges.begin(); it_edge != sorted_edges.end();) {
            Edge& edge = *it_edge;
            edge.is_checking = 1; // Đánh dấu đang kiểm tra cạnh này
            history.push_back(sorted_edges); // Lưu lại trạng thái hiện tại của MST trước khi thêm cạnh mới
            edge.is_checking = 0; // Bỏ đánh dấu kiểm tra sau khi đã lưu trạng thái
            if (unite(edge.u_idx, edge.v_idx)) {
                edge.is_focused = 1; // Đánh dấu cạnh này là một phần của MST
                history.push_back(sorted_edges); // Lưu lại trạng thái hiện tại của MST sau khi thêm cạnh mới
                ++it_edge;
            } else {
                it_edge = sorted_edges.erase(it_edge);
            }
        }

        history.push_back(sorted_edges); // Lưu lại trạng thái cuối cùng của MST sau khi hoàn thành thuật toán
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
            G.edges = history[curent_state];
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
            } else if (opp.command == "Kruskal") {
                find_mst();
            }
            opp.is_pending = false;
        }
    }

    std::string Run() {
        opp.initialize(20, 700, 0, false);
        cam.initialize();
        speed.initialize(850, 800, 100, 50);
        list_data.init(20, 50);
        list_data.choose = "Kruskal";
        list_data.focused = 0;
        int screenWidth = 1000;
        int screenHeight = 600; 

        progress_duration.initialize(550, 800, 200, 0);

        while (!WindowShouldClose()) {
            if (list_data.choose != "Kruskal") {
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
                    G.edges = history[++curent_state];
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