#include "raylib.h"
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include "camera.h" // Giả định file này đã có sẵn của bạn

#pragma once

const float NODE_RADIUS = 40.0f;
const float SIBLING_SEP = 150.0f;   // Khoảng cách giữa các anh em
const float SUBTREE_SEP = 200.0f;   // Khoảng cách tối thiểu giữa các bụi cây
const float LEVEL_SEP = 150.0f;    // Khoảng cách giữa các tầng

struct TrieNode {
    char value;
    int id;
    int depth = 0;
    int parentIdx = -1;
    bool isEndOfWord = false;
    bool is_checking = false; // Biến để đánh dấu node đang được kiểm tra trong quá trình tìm kiếm
    bool is_focused = false; // Biến để đánh dấu node đang được focus trong quá trình tìm kiếm
    std::vector<int> children;
    int cnt = 0;
    // Tọa độ và biến phục vụ thuật toán layout
    float x = 0, y = 0, mod = 0;

    TrieNode(char c, int _id, int d, int p) : value(c), id(_id), depth(d), parentIdx(p) {}
};

class Trie {
public:
    std::vector<TrieNode> tree;
    std::vector <std::vector <TrieNode>> history;
    const float smoothSpeed = 0.1f;
    float time_per_change = 0.5, progress = 0;
    int curent_state = 0;
    duration progress_duration, speed; 
    camera cam;
    operation opp;
    menu list_data;

    
    int rootIdx = -1;

    Trie() {
        // Khởi tạo root trống
        tree.emplace_back(' ', 0, 0, -1);
        rootIdx = 0;
    }

    void insert(std::string word) {
        history.push_back(tree);
        int curr = rootIdx;
        for (char c : word) {
            int next = -1;
            for (int childId : tree[curr].children) {
                if (tree[childId].value == c) {
                    next = childId;  
                    tree[childId].is_checking = true; 
                    history.push_back(tree); 
                    tree[childId].is_checking = false; // Tắt hiệu ứng kiểm tra sau khi đã lưu trạng thái
                    break;
                }
            }
            if (next == -1) { 
                int newId = (int)tree.size();
                tree.emplace_back(c, newId, tree[curr].depth + 1, curr);
                tree[curr].children.push_back(newId);
                curr = newId;
                UpdateLayout();
                history.push_back(tree);
                tree[curr].is_checking = true;
                history.push_back(tree);
                tree[curr].is_checking = false;
            } else curr = next;
        }

        if (search(word) == false) {
            curr = rootIdx;
            tree[curr].cnt++;
            for (char c : word) {
                int next = -1;
                for (int childId : tree[curr].children) {
                    if (tree[childId].value == c) {
                        next = childId;
                        break;
                    }
                }
                curr = next;
                tree[curr].cnt++;
            }
        }

        tree[curr].isEndOfWord = true;
        history.push_back(tree);
    }

    // --- REINGOLD-TILFORD ALGORITHM ---

    // Lấy mép trái/phải của một subtree tại từng độ sâu
    void GetContour(int nodeIdx, float modSum, std::map<int, float>& contour, bool isLeft) {
        float currX = tree[nodeIdx].x + modSum;
        
        if (contour.find(tree[nodeIdx].depth) == contour.end()) {
            contour[tree[nodeIdx].depth] = currX;
        } else {
            if (isLeft) contour[tree[nodeIdx].depth] = std::min(contour[tree[nodeIdx].depth], currX);
            else contour[tree[nodeIdx].depth] = std::max(contour[tree[nodeIdx].depth], currX);
        }

        for (int childId : tree[nodeIdx].children) {
            GetContour(childId, modSum + tree[nodeIdx].mod, contour, isLeft);
        }
    }

    void ResolveConflicts(int nodeIdx) {
        int pIdx = tree[nodeIdx].parentIdx;
        if (pIdx == -1) return;

        auto& parent = tree[pIdx];
        int nodePos = 0;
        for (int i = 0; i < parent.children.size(); ++i) {
            if (parent.children[i] == nodeIdx) { nodePos = i; break; }
        }

        float shiftNeeded = 0;
        std::map<int, float> currentLeftContour;
        GetContour(nodeIdx, 0, currentLeftContour, true);

        // Kiểm tra với tất cả các nhánh bên trái
        for (int i = 0; i < nodePos; ++i) {
            std::map<int, float> sibRightContour;
            GetContour(parent.children[i], 0, sibRightContour, false);

            for (auto  v : sibRightContour) {
                float depth = v.first, rightX = v.second;
                if (currentLeftContour.count(depth)) {
                    float overlap = (rightX + SUBTREE_SEP) - currentLeftContour[depth];
                    if (overlap > shiftNeeded) shiftNeeded = overlap;
                }
            }
        }

        if (shiftNeeded > 0) {
            tree[nodeIdx].x += shiftNeeded;
            tree[nodeIdx].mod += shiftNeeded;
        }
    }

    void FirstPass(int nodeIdx) {
        auto& node = tree[nodeIdx];
        if (node.children.empty()) {
            int leftSibIdx = -1;
            int pIdx = node.parentIdx;
            if (pIdx != -1) {
                for (int i = 1; i < tree[pIdx].children.size(); ++i) {
                    if (tree[pIdx].children[i] == nodeIdx) leftSibIdx = tree[pIdx].children[i-1];
                }
            }
            node.x = (leftSibIdx != -1) ? tree[leftSibIdx].x + SIBLING_SEP : 0;
        } else {
            for (int childId : node.children) FirstPass(childId);

            float midpoint = (tree[node.children.front()].x + tree[node.children.back()].x) / 2.0f;
            int leftSibIdx = -1;
            int pIdx = node.parentIdx;
            if (pIdx != -1) {
                for (int i = 1; i < tree[pIdx].children.size(); ++i) {
                    if (tree[pIdx].children[i] == nodeIdx) leftSibIdx = tree[pIdx].children[i-1];
                }
            }

            if (leftSibIdx != -1) {
                node.x = tree[leftSibIdx].x + SIBLING_SEP;
                node.mod = node.x - midpoint;
                ResolveConflicts(nodeIdx);
            } else {
                node.x = midpoint;
            }
        }
    }

    void SecondPass(int nodeIdx, float cumMod, float& xMin, Vector2 offset = {0, 0}) {
        auto& node = tree[nodeIdx];
        node.x += cumMod + offset.x;
        node.y = node.depth * LEVEL_SEP + offset.y;

        if (node.x < xMin) xMin = node.x;
        for (int childId : node.children) SecondPass(childId, cumMod + node.mod, xMin, offset);
    }

    void UpdateLayout() {
        if (tree.empty()) return;
        //for (auto& n : tree) { n.x = 0; n.mod = 0; }
        
        FirstPass(rootIdx);
        Vector2 offset = {GetScreenWidth() / 2.0f - tree[rootIdx].x, -tree[rootIdx].y + 50.f};
        //float xMin = 0;
        float xMin = 0;
        SecondPass(rootIdx, 0, xMin, offset);
        
        // Căn lề trái để x không bị âm
        //for (auto& n : tree) n.x -= xMin;
    }

    void draw() {
        list_data.update();
        opp.update();
        speed.update();

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
            tree = history[curent_state];
        }

        if (speed.is_changed) {
            time_per_change = 0.05f + (100 - speed.value) / 50;
            speed.is_changed = false;
        }

        if (curent_state >= (int)history.size() - 1) {
            for (auto& node : tree) {
                Vector2 pos = { node.x, node.y};
                for (int childId : node.children) {
                    Vector2 childPos = { tree[childId].x , tree[childId].y};
                    DrawLineEx(pos, childPos, 5.0f, GRAY);
                }
                
                DrawCircleV(pos, NODE_RADIUS, node.isEndOfWord ? PINK : SKYBLUE);
                Vector2 textSize = MeasureTextEx(GetFontDefault(), std::string(1, node.value).c_str(), 30, 1);
                DrawTextEx(font, std::string(1, node.value).c_str(), {pos.x - textSize.x / 2, pos.y - textSize.y / 2}, 30, 1, WHITE);
            }
        } else {
            // Vẽ trạng thái trung gian nếu đang trong quá trình chuyển đổi
            auto& currentTree = history[curent_state];
            auto& nextTree = history[curent_state + 1];
            for (int i = 0; i < currentTree.size(); ++i) {
                Vector2 pos = { currentTree[i].x, currentTree[i].y};
                Vector2 nextPos = { nextTree[i].x, nextTree[i].y};
                Vector2 interpPos = Vector2Lerp(pos, nextPos, progress / time_per_change);

                if (nextTree[i].is_checking) {
                    DrawRing(interpPos, NODE_RADIUS - 5, NODE_RADIUS + 5, 0, 360, 1, RED);
                }

                for (int childId : currentTree[i].children) {
                    Vector2 childPos = { currentTree[childId].x , currentTree[childId].y};
                    Vector2 nextChildPos = { nextTree[childId].x , nextTree[childId].y};
                    Vector2 interpChildPos = Vector2Lerp(childPos, nextChildPos, progress / time_per_change);
                    DrawLineEx(interpPos, interpChildPos, 5.0f, GRAY);
                }
                
                DrawCircleV(interpPos, NODE_RADIUS, currentTree[i].isEndOfWord ? PINK : SKYBLUE);
                Vector2 textSize = MeasureTextEx(GetFontDefault(), std::string(1, currentTree[i].value).c_str(), 30, 1);
                DrawTextEx(font, std::string(1, currentTree[i].value).c_str(), {interpPos.x - textSize.x / 2, interpPos.y - textSize.y / 2}, 30, 1, WHITE);
            }

            for (int i = currentTree.size(); i < nextTree.size(); ++i) {
                Vector2 pos = { nextTree[i].x, nextTree[i].y - 100};
                Vector2 nextPos = { nextTree[i].x, nextTree[i].y};
                Vector2 interpPos = Vector2Lerp(pos, nextPos, progress / time_per_change);
                DrawCircleV(interpPos, NODE_RADIUS, nextTree[i].isEndOfWord ? PINK : SKYBLUE);
                Vector2 textSize = MeasureTextEx(GetFontDefault(), std::string(1, nextTree[i].value).c_str(), 30, 1);
                DrawTextEx(font, std::string(1, nextTree[i].value).c_str(), {interpPos.x - textSize.x / 2, interpPos.y - textSize.y / 2}, 30, 1, WHITE);
                if (nextTree[i].is_checking) {
                    DrawRing(interpPos, NODE_RADIUS - 5, NODE_RADIUS + 5, 0, 360, 1, Fade(RED, progress / time_per_change));
                }
            }
        }
    }

    bool search(std::string word) {
        int curr = rootIdx;
        for (char c : word) {
            int next = -1;
            for (int childId : tree[curr].children) {
                if (tree[childId].value == c) {
                    next = childId;  
                    break;
                }
            }
            if (next == -1) return false; // Từ không tồn tại
            curr = next;
        }
        return tree[curr].isEndOfWord; // Trả về true nếu node cuối cùng đánh dấu kết thúc từ
    }

    void draw_task() {
        progress_duration.draw();
        opp.draw();
        speed.draw();
        list_data.draw();
    }

    void do_insert(std::string word) {
        insert(word);
        UpdateLayout();
        history.push_back(tree);
    }

    void delete_node(std::string word) {
        if (search(word) == false) return; 
        int curr = rootIdx;
        tree[curr].cnt--;
        for (char c : word) {
            int next = -1;
            for (int childId : tree[curr].children) {
                if (tree[childId].value == c) {
                    next = childId;  
                    break;
                }
            }
            curr = next;
            tree[curr].cnt--;
        }
        tree[curr].isEndOfWord = false;
    }

    void do_delete_node(std::string word) {
        // Xóa node đánh dấu kết thúc từ, không xóa node thực tế để tránh phức tạp
        int curr = rootIdx;
        for (char c : word) {
            int next = -1;
            for (int childId : tree[curr].children) {
                if (tree[childId].value == c) { next = childId; break; }
            }
            if (next == -1) return; // Từ không tồn tại
            curr = next;
        }
        tree[curr].isEndOfWord = false;
    }

    void do_search(std::string word) {
        history.push_back(tree);
        int curr = rootIdx;
        for (char c : word) {
            int next = -1;
            for (int childId : tree[curr].children) {
                if (tree[childId].value == c) {
                    next = childId;  
                    tree[childId].is_checking = true; 
                    history.push_back(tree); 
                    tree[childId].is_checking = false; // Tắt hiệu ứng kiểm tra sau khi đã lưu trạng thái
                    history.push_back(tree); // Lưu trạng thái sau khi tắt hiệu ứng kiểm tra
                    break;
                }
            }
            if (next == -1) return; // Từ không tồn tại
            curr = next;
        }
        // Có thể thêm hiệu ứng nếu tree[curr].isEndOfWord == true
    }

    void update() {
        if (opp.is_pending) {
            if (opp.command == "Insert") {
                do_insert(opp.str_value);
            } else if (opp.command == "Delete") {
                do_delete_node(opp.str_value);
            } else if (opp.command == "Search") {
                do_search(opp.str_value);
            }
            opp.is_pending = false;
        }
    }

    std::string Run() {
        SetTargetFPS(60);
        cam.initialize();
        opp.initialize(20, 700, 1);
        speed.initialize(850, 800, 100, 50);
        progress_duration.initialize(550, 800, 200, 0);
        list_data.init(20, 50);
        list_data.choose = "Trie";
        list_data.focused = 0;
        int screenWidth = 1000;
        int screenHeight = 600; 

        // Test data
        for (int i = 0; i < 10; ++i) {
            int l = GetRandomValue(1, 10);
            std::string word;
            for (int j = 0; j < l; ++j) {
                word += 'a' + GetRandomValue(0, 25);
            }
            insert(word);   
        }
        
        UpdateLayout();

        while (!WindowShouldClose()) {
            if (list_data.choose != "Trie") {
                return list_data.choose;
            }

            cam.update_zoom();
            if (progress_duration.isDraggingHandle == 0 && speed.isDraggingHandle == 0) {
                cam.update_dragging();
            }

            if (curent_state + 1 < history.size()) {
                progress += GetFrameTime();
                if (progress > time_per_change) {
                    progress = 0;
                    tree = history[++curent_state];
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