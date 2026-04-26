#include "kruskal.h"
#pragma once


/*
    std::vector <std::string> kruskal;
    kruskal.push_back("sort edges by weight");
    kruskal.push_back("for (edge in edges)");
    kruskal.push_back("    if (find(edge.u) != find(edge.v)));
    kruskal.push_back("        union(edge.u, edge.v);");    
    kruskal.push_back("        add edge to MST;");
    kruskal.push_back("    else erase edge from consideration");
*/

void kruskal::find_mst() {
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

    snippets.push_back(0);
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
        snippets.push_back(2); // Thêm đoạn mã tương ứng với bước này (có thể là "Checking edge (u, v) with weight w")
        edge.is_checking = 0; // Bỏ đánh dấu kiểm tra sau khi đã lưu trạng thái
        if (unite(edge.u_idx, edge.v_idx)) {
            edge.is_focused = 1; // Đánh dấu cạnh này là một phần của MST
            history.push_back(sorted_edges); // Lưu lại trạng thái hiện tại của MST sau khi thêm cạnh mới
            snippets.push_back(4); // Thêm đoạn mã tương ứng với bước này (có thể là "Added edge (u, v) to MST")
            ++it_edge;
        } else {
            it_edge = sorted_edges.erase(it_edge);
            history.push_back(sorted_edges); // Lưu lại trạng thái hiện tại của MST sau khi loại bỏ cạnh này khỏi consideration
            snippets.push_back(5); 
        }
    }

    history.push_back(sorted_edges); // Lưu lại trạng thái cuối cùng của MST sau khi hoàn thành thuật toán
    snippets.push_back(-1); // Thêm đoạn mã tương ứng với bước này (có thể là "Kruskal's algorithm completed")
    history.push_back(history[0]);
}

void kruskal::draw() {
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

void kruskal::draw_task() {
    progress_duration.draw();
    speed.draw();
    opp.draw();
    G.UpdateGraphLayout();
    assets::Instance().draw_texture("home_icon", {(float)(GetScreenWidth() - 100), 10}, 0.3f);
    if (snippets.empty()) return;
    highlight_code::Instance().draw("kruskal", snippets[std::min(curent_state, (int)snippets.size() - 1)]);
}

void kruskal::update() {
    if (opp.is_pending) {
        snippets.clear();
        history.clear();
        curent_state = 0; progress = 0;
        progress_duration.value = 0;
        if (opp.command == "Vertices") {
            vertices = stoi(opp.str_value);
            G.gen_random(vertices, edges);
        } else if (opp.command == "Edges") {
            edges = stoi(opp.str_value);
            G.gen_random(vertices, edges);
        } else if (opp.command == "Intit") {
            vertices = GetRandomValue(10, 20);
            edges = GetRandomValue(10,vertices * (vertices - 1) / 3);
            G.gen_random(vertices, edges);
        } else if (opp.command == "Kruskal") {
            find_mst();
        } else if (opp.command == "Random") {
            std::string str = opp.str_value;
            std::pair <int, int> val = {0, 0};
            for (int i = 0; i < str.size(); i++) {
                if (str[i] == ' ') {
                    val.first = stoi(str.substr(0, i));
                    val.second = stoi(str.substr(i + 1));
                    break;
                }
            }

            vertices = val.first;
            edges = val.second;
            edges = std::min(edges, vertices * (vertices - 1) / 2); // Giới hạn số cạnh tối đa
            G.gen_random(vertices, edges);
        } else if (opp.command == "Add Edges") {
            std::string str = opp.str_value;
            std::tuple<int, int, int> new_edges;
            std::string edge_str = str.substr(0, str.length());
            size_t first_space = edge_str.find(' ');
            size_t second_space = edge_str.find(' ', first_space + 1);
            if (first_space != std::string::npos && second_space != std::string::npos) {
                int u = stoi(edge_str.substr(0, first_space));
                int v = stoi(edge_str.substr(first_space + 1, second_space - first_space - 1));
                int w = stoi(edge_str.substr(second_space + 1));
                new_edges = {u, v, w};
            }

            bool edge_exists = false;
            for (auto &edge : G.edges) {
                if ((edge.u_idx == std::get<0>(new_edges) && edge.v_idx == std::get<1>(new_edges)) || (edge.u_idx == std::get<1>(new_edges) && edge.v_idx == std::get<0>(new_edges))) {
                    edge.length = std::get<2>(new_edges); // Cập nhật trọng số nếu cạnh đã tồn tại
                    edge_exists = true;
                    break;
                }
            }

            if (!edge_exists) {
                if (std::get<0>(new_edges) >= 0 && std::get<0>(new_edges) < G.nodes.size() && std::get<1>(new_edges) >= 0 && std::get<1>(new_edges) < G.nodes.size()) {
                    G.edges.push_back({std::get<0>(new_edges), std::get<1>(new_edges), std::get<2>(new_edges)}); // Thêm cạnh mới nếu chưa tồn tại
                }
            }
        } else if (opp.command == "From File") {
            G.build_from_file(opp.str_value);
        }

        opp.appear_sub_option = false;
        opp.is_pending = false;
    }
}

std::string kruskal::Run() {
    cam.initialize();
    opp.initialize(20, 700, 0, false);
    
    list_data.init(20, 50);
    list_data.choose = "Kruskal";
    list_data.focused = 0;
    int screenWidth = 1000;
    int screenHeight = 600; 

    speed.initialize(1350, 800, 100, 50);
    progress_duration.initialize(1000, 800, 200, 0);

    while (!WindowShouldClose()) {
        if (assets::Instance().is_clicked({(float)(GetScreenWidth() - 100), 10}, "home_icon", 0.3f)) {
            break;
        }
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
    return "";
}