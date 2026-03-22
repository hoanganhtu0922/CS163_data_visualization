#include "raylib.h"
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include "camera.h" // Giả định file này đã có sẵn của bạn
#include "operation.h" // Giả định file này đã có sẵn của bạn
#include "menu.h" // Giả định file này đã có sẵn của bạn
#include "duration.h" // Giả định file này đã có sẵn của bạn
#include "raymath.h"
#pragma once

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

struct Trie {
    const float NODE_RADIUS = 40.0f;
    const float SIBLING_SEP = 150.0f;   // Khoảng cách giữa các anh em
    const float SUBTREE_SEP = 200.0f;   // Khoảng cách tối thiểu giữa các bụi cây
    const float LEVEL_SEP = 150.0f;    // Khoảng cách giữa các tầng
    std::vector<TrieNode> tree;
    std::vector <std::vector <TrieNode>> history;
    float smoothSpeed = 0.1f;
    float time_per_change = 0.5, progress = 0;
    int curent_state = 0;
    duration progress_duration, speed; 
    camera cam;
    operation opp;
    menu list_data;
    int rootIdx = -1;

    Trie() {
        tree.emplace_back(' ', 0, 0, -1);
        rootIdx = 0;
    }

    void insert(std::string word);

    // --- REINGOLD-TILFORD ALGORITHM ---

    void GetContour(int nodeIdx, float modSum, std::map<int, float>& contour, bool isLeft);
    void ResolveConflicts(int nodeIdx);
    bool is_empty(int node);
    void FirstPass(int nodeIdx);
    void SecondPass(int nodeIdx, float cumMod, float& xMin, Vector2 offset = {0, 0});
    void UpdateLayout();
    void draw();
    bool search(std::string word);
    void draw_task();
    void do_insert(std::string word);
    void delete_node(std::string word);
    void do_delete_node(std::string word);
    void do_search(std::string word);
    void update();
    std::string Run();
};