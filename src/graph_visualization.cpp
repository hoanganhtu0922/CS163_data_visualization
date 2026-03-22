#include "graph_visualization.h"

bool graph::is_dragging() {
    for (auto& node : nodes) {
        if (node.update_dragging(cam)) {
            return true; // Nếu có node nào đang được kéo, trả về true
        }
    }
    return false; // Không có node nào đang được kéo
}

void graph::UpdateGraphLayout() {
    float area = 1500.0f * 900.0f; // Diện tích màn hình
    int N = nodes.size();
    if (N > 0) {
        // Cập nhật lại c2 dựa trên số lượng node
        // 0.5f là hằng số điều chỉnh (C)
        c2 = 0.5f * sqrtf(area / N); 
        
        // Giới hạn c2 để không quá nhỏ hoặc quá lớn
        if (c2 < 50.0f) c2 = 50.0f;
        if (c2 > 200.0f) c2 = 200.0f;
    }
    
    // 1. Reset Forces
    for (auto& node : nodes) node.force = {0, 0};

    for (auto& node : nodes) {
        node.update_dragging(cam);
    }
    // 2. Repulsive Forces (Between ALL nodes)
    for (int i = 0; i < nodes.size(); i++) {
        for (int j = i + 1; j < nodes.size(); j++) {
            Vector2 dir = Vector2Subtract(nodes[j].pos, nodes[i].pos);
            float dist = Vector2Length(dir);
            if (dist < 0.1) dist = 0.1; // Prevent division by zero

            float forceMag = c3 / (dist * dist);
            Vector2 forceVec = Vector2Scale(Vector2Normalize(dir), forceMag);

            nodes[j].force = Vector2Add(nodes[j].force, forceVec);
            nodes[i].force = Vector2Subtract(nodes[i].force, forceVec);
        }
    }

    // 3. Attractive Forces (Only between connected nodes)
    for (const auto& edge : edges) {
        Node_graph& u = nodes[edge.u_idx];
        Node_graph& v = nodes[edge.v_idx];

        Vector2 dir = Vector2Subtract(v.pos, u.pos);
        float dist = Vector2Length(dir);
        if (dist < 0.1) dist = 0.1;

        float forceMag = c1 * log(dist / c2);
        Vector2 forceVec = Vector2Scale(Vector2Normalize(dir), forceMag);

        u.force = Vector2Add(u.force, forceVec);
        v.force = Vector2Subtract(v.force, forceVec);
    }

    // 4. Move Nodes
    for (auto& node : nodes) {
        node.pos.x += c4 * node.force.x;
        node.pos.y += c4 * node.force.y;
    }
}

void graph::draw() {
    for (const auto& edge : edges) {
            Vector2 start = nodes[edge.u_idx].pos;
            Vector2 end = nodes[edge.v_idx].pos;

            // 1. Vẽ đường nối
            Color color_text = BLACK;
            DrawLineEx(start, end, 4.0f, GRAY);

            if (edge.is_focused) {
                DrawLineEx(start, end, 4.0f, SKYBLUE);
                color_text = SKYBLUE;
            } else if (edge.is_checking) {
                DrawLineEx(start, end, 4.0f, PINK);
                color_text = PINK;
            }

            if (std::make_pair(start.x, start.y) >= std::make_pair(end.x, end.y)) {
                std::swap(start, end); // Đảm bảo start luôn là điểm bên trái để text không bị lộn ngược
            }

            Vector2 midPoint = Vector2Lerp(start, end, 0.5f);
            Vector2 diff = Vector2Subtract(end, start);
            
            float angle = atan2f(diff.y, diff.x) * RAD2DEG;

            std::string distText = std::to_string(edge.length);
            float fontSize = 20.0f; // Tăng lên tí cho dễ nhìn
            float spacing = 2.0f;
            Vector2 textSize = MeasureTextEx(font, distText.c_str(), fontSize, spacing);

            // 4. Origin: Đặt là tâm của text để xoay tại chỗ
            // Chúng ta trừ textSize.y đi một chút để chữ nổi lên trên sợi dây
            Vector2 textOrigin = { textSize.x / 2.0f, textSize.y + 5.0f }; 
            //if (angle < 0) {
            DrawTextPro(font, distText.c_str(), midPoint, textOrigin, angle, fontSize, spacing, color_text);
            //}
            // 5. Vẽ
            
        }

    for (const auto& node : nodes) {
        DrawCircleV(node.pos, 30.0f, SKYBLUE);
        if (node.is_focused) {
            DrawCircleV(node.pos, 30.0f, PINK);
        } 
        std::string num_str = std::to_string(node.id);
        Vector2 text = MeasureTextEx(font, num_str.c_str(), 20, 1);
        DrawTextEx(font, num_str.c_str(), {node.pos.x - text.x / 2, node.pos.y - text.y / 2}, 20, 1, BLACK);
        if (node.is_checking) {
            DrawRing(node.pos, 30.0f - 3, 30.0f + 3, 0, 360, 1, RED);
        }
    }
}

void graph::gen_random(int n, int m) {
    nodes.clear();
    edges.clear();
    m = std::min(m, n * (n - 1) / 2); // Giới hạn số cạnh tối đa trong đồ thị vô hướng đơn giản
    for (int i = 0; i < n; i++) {
        Node_graph node;
        node.pos = { (float)GetRandomValue(100, 1400), (float)GetRandomValue(100, 800) };
        node.id = i;
        nodes.push_back(node);
    }

    std::set <std::pair<int, int>> existing_edges; // Để tránh trùng lặp cạnh
    for (int i = 0; i < m; i++) {
        int u = GetRandomValue(0, n - 1);
        int v = GetRandomValue(0, n - 1);
        if (u != v && existing_edges.find({u, v}) == existing_edges.end() && existing_edges.find({v, u}) == existing_edges.end()) {
            existing_edges.insert({u, v});
            Edge edge;
            edge.u_idx = u;
            edge.v_idx = v;
            edge.length = GetRandomValue(1, 100);
            edges.push_back(edge);
        }
    }    
}


