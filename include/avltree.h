#include "raylib.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include "camera.h" // Giả định file này đã có sẵn của bạn
#include "operation.h" // Giả định file này đã có sẵn của bạn
#include "menu.h" // Giả định file này đã có sẵn của bạn
#include "duration.h" // Giả định file này đã có sẵn của bạn
#include "raymath.h"
#include "assets.h"
#include "highlight_code.h"
#include <iostream>
#include <fstream>
#pragma once

struct avl_Node {
    int data;
    avl_Node *left, *right;
    int height = 0, is_checking = 0, is_focused = 0;
    avl_Node(int val) : data(val), left(nullptr), right(nullptr), height(1), is_checking(false), is_focused(false) {}
};

struct node_rep{
    int data;
    float x, y, lefval = -1, rigval = -1;
    int exist = 0;
    bool is_checking = 0, is_focused = 0;
};

struct AVLTree {
    const float NODE_RADIUS = 40.0f;
    const float NODE_SEP = 60.0f;   // Khoảng cách giữa các anh em
    const float LEVEL_SEP = 80.0f;    // Khoảng cách giữa các tầng
    std::vector <node_rep> tree; // Store node representations
    std::vector <std::vector <node_rep>> history; // Store history of tree states for animation 
    std::vector <int> snippets; // Store code snippet indices for current operation
    avl_Node *root = nullptr;

    const float smoothSpeed = 0.1f;
    float time_per_change = 0.5, progress = 0;
    int curent_state = 0;
    int split_point = 0;
    duration progress_duration, speed; 
    camera cam;
    operation opp;
    std::string command_type;

    void dfs(avl_Node* node, int depth, float SEP, float x);
    
    void update_layout();

    void do_insert(int value, int is_reset = 0);

    void do_delete_node(int value);

    void do_search(int value);

    void update();

    void build_from_str(std::string str);
    // --- Helpers ---
    int getHeight(avl_Node* n);

    int getBalanceFactor(avl_Node* n);

    void updateHeight(avl_Node* n);

    // --- Rotations (Modify by Reference) ---
    
    void rotateRight(avl_Node*& y);
    void rotateLeft(avl_Node*& x);

    void save_history(int id);

    // Consolidated balancing logic
    void balance(avl_Node*& node);

    // --- Core Operations ---

    void insert(avl_Node*& node, int val);

    void remove(avl_Node*& node, int val);
    bool search(avl_Node* node, int val);

    void draw();

    void draw_task();

    std::string Run();
};