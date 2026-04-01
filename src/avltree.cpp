#include "avltree.h"

void AVLTree::dfs(avl_Node* node, int depth, float SEP, float x) {
    if (node == nullptr) return;
    tree[node -> data] = {node->data, x, depth * LEVEL_SEP, -1, -1, 1, 0, 0};
    tree[node -> data].is_checking = node->is_checking;
    tree[node -> data].is_focused = node->is_focused;
    dfs(node->left, depth + 1, SEP / 2.0f, x - SEP / 2.0f);
    dfs(node->right, depth + 1, SEP / 2.0f, x + SEP / 2.0f);
    if (node -> left) tree[node -> data].lefval = node->left->data;
    if (node -> right) tree[node -> data].rigval = node->right->data;
}

void AVLTree::update_layout() {
    for (int i = 0; i < tree.size(); ++i) {
        tree[i].exist = 0; // Reset all nodes
    }

    if (root == nullptr) {
        return;
    }

    tree[root -> data].x = GetScreenWidth() / 2; // Start with root in the center
    tree[root -> data].y = 50; // Start with root at the top

    int height = getHeight(root);
    float SEP = NODE_SEP;
    for (int i = 1; i < height; ++i) SEP *= 2.0f;
    dfs(root, 0, SEP, 0);
}

void AVLTree::do_insert(int value) {
    curent_state = 0;
    progress = 0;
    history.clear();
    snippets.clear();
    save_history(-1);
    insert(root, value);
    save_history(-1);
}

void AVLTree::do_delete_node(int value) {
    curent_state = 0;
    progress = 0;
    history.clear();
    snippets.clear();
    save_history(-1);
    remove(root, value);
    save_history(-1);
}

void AVLTree::do_search(int value) {
    curent_state = 0;
    progress = 0;
    history.clear();
    snippets.clear();
    save_history(-1);
    search(root, value);
    save_history(-1);
}

void AVLTree::update() {
    if (opp.is_pending) {
        if (opp.command == "Insert") {
            command_type = "avl_insert";
            do_insert(stoi(opp.str_value));
        } else if (opp.command == "Delete") {
            command_type = "avl_delete";
            do_delete_node(stoi(opp.str_value));
        } else if (opp.command == "Search") {
            command_type = "avl_search";
            do_search(stoi(opp.str_value));
        }
        opp.is_pending = false;
    }
}

// --- Helpers ---
int AVLTree::getHeight(avl_Node* n) { return n ? n->height : 0; }

int AVLTree::getBalanceFactor(avl_Node* n) { 
    return n ? getHeight(n->left) - getHeight(n->right) : 0; 
}

void AVLTree::updateHeight(avl_Node* n) {
    if (n) n->height = 1 + std::max(getHeight(n->left), getHeight(n->right));
}

// --- Rotations (Modify by Reference) ---

void AVLTree::rotateRight(avl_Node*& y) {
    avl_Node* x = y->left;
    avl_Node* T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    updateHeight(y);
    updateHeight(x);

    // Update the actual pointer in the parent/root
    y = x; 
}

void AVLTree::rotateLeft(avl_Node*& x) {
    avl_Node* y = x->right;
    avl_Node* T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    updateHeight(x);
    updateHeight(y);

    // Update the actual pointer in the parent/root
    x = y;
}

void AVLTree::save_history(int id) {
    update_layout();
    history.push_back(tree);
    snippets.push_back(id);
}

/*
    std::vector <std::string> avl_insert;
    avl_insert.push_back("if (value < root->data)");
    avl_insert.push_back("    root->left = insert(root->left, value)");
    avl_insert.push_back("else if (value > root->data)");
    avl_insert.push_back("    root->right = insert(root->right, value)");
    avl_insert.push_back("else return root; ");
    avl_insert.push_back("Balance the tree and return the new root");
    code_snippets["avl_insert"] = avl_insert;

    std::vector <std::string> avl_delete;
    avl_delete.push_back("if (value < root->data)");
    avl_delete.push_back("    deleteNode(root->left, value)");
    avl_delete.push_back("else if (value > root->data)");
    avl_delete.push_back("    deleteNode(root->right, value)");
    avl_delete.push_back("else");
    avl_delete.push_back("    if (root->left == NULL || root->right == NULL)");
    avl_delete.push_back("        delete root");
    avl_delete.push_back("    else");
    avl_delete.push_back("        find inorder successor");
    avl_delete.push_back("        swap data with successor");
    avl_delete.push_back("Balance the tree and return the new root");
    code_snippets["avl_delete"] = avl_delete;

    std::vector <std::string> avl_search;
    avl_search.push_back("if (root == NULL) return NULL;");
    avl_search.push_back("if (value < root->data)");
    avl_search.push_back("    return search(root->left, value);");
    avl_search.push_back("else if (value > root->data)");
    avl_search.push_back("    return search(root->right, value);");
    avl_search.push_back("else return root;");
    code_snippets["avl_search"] = avl_search;

    std::vector <std::string> avl_update;
    avl_update.push_back("node target = search(root, oldValue);");
    avl_update.push_back("if (target != NULL)");
    avl_update.push_back("    deleteNode(root, oldValue);");
    avl_update.push_back("    root = insert(root, newValue);");
    code_snippets["avl_update"] = avl_update;
*/

// Consolidated balancing logic

void AVLTree::balance(avl_Node*& node) {
    if (!node) return;

    updateHeight(node);
    int bFactor = getBalanceFactor(node);

    // Left Heavy
    if (bFactor > 1) {
        if (getBalanceFactor(node->left) < 0) {
            rotateLeft(node->left); // Left-Right Case
            save_history(highlight_code::Instance().find_snippet(command_type, "Balance")); // Highlight rotation step
        }
        rotateRight(node);
        save_history(highlight_code::Instance().find_snippet(command_type, "Balance")); // Highlight rotation step
    }
    // Right Heavy
    else if (bFactor < -1) {
        if (getBalanceFactor(node->right) > 0) {
            rotateRight(node->right); // Right-Left Case
            save_history(highlight_code::Instance().find_snippet(command_type, "Balance")); // Highlight rotation step
        }
        rotateLeft(node);
        save_history(highlight_code::Instance().find_snippet(command_type, "Balance")); // Highlight rotation step
    }
}

// --- Core Operations ---

/*
    std::vector <std::string> avl_insert;
    avl_insert.push_back("if (root == NULL) root = newNode(value)");
    avl_insert.push_back("if (value < root->data)");
    avl_insert.push_back("    root->left = insert(root->left, value)");
    avl_insert.push_back("else if (value > root->data)");
    avl_insert.push_back("    root->right = insert(root->right, value)");
    avl_insert.push_back("else return root; ");
    avl_insert.push_back("Balance the tree and return the new root");
    code_snippets["avl_insert"] = avl_insert;
*/

void AVLTree::insert(avl_Node*& node, int val) {
    if (node == nullptr) {
        node = new avl_Node(val);
        save_history(0);
        return;
    }

    node->is_checking = true; // Highlight node being checked
    if (val < node->data) {
        save_history(2);
    } else if (val > node->data) {
        save_history(4);
    }

    node->is_checking = false; // Remove highlight after recording state

    if (val < node->data)      insert(node->left, val);
    else if (val > node->data) insert(node->right, val);
    else return; // Duplicate

    balance(node);
}

/*
    std::vector <std::string> avl_delete;
    avl_delete.push_back("if (value < root->data)");
    avl_delete.push_back("    deleteNode(root->left, value)");
    avl_delete.push_back("else if (value > root->data)");
    avl_delete.push_back("    deleteNode(root->right, value)");
    avl_delete.push_back("else");
    avl_delete.push_back("    if (root->left == NULL || root->right == NULL)");
    avl_delete.push_back("        delete root");
    avl_delete.push_back("    else");
    avl_delete.push_back("        find inorder successor");
    avl_delete.push_back("        swap data with successor");
    avl_delete.push_back("Balance the tree and return the new root");
    code_snippets["avl_delete"] = avl_delete;
*/
void AVLTree::remove(avl_Node*& node, int val) {
    if (!node) return;

    node->is_checking = true; // Highlight node being checked
    if (val < node->data) {
        save_history(1);
    } else if (val > node->data) {
        save_history(3);
    }
    node->is_checking = false; // Remove highlight after recording state

    if (val < node->data) {
        remove(node->left, val);
    } else if (val > node->data) {
        remove(node->right, val);
    } else {
        // Node with only one child or no child
        if (!node->left || !node->right) {
            avl_Node* temp = node->left ? node->left : node->right;
            
            if (!temp) { // No child case
                temp = node;
                node = nullptr; 
            } else { // One child case
                *node = *temp; // Copy content
            }
            delete temp;
            save_history(6); // Highlight deletion step
        } else {
            // Node with two children: Get inorder successor
            avl_Node* temp = node->right;
            temp->is_checking = true; // Highlight successor
            save_history(9);
            temp->is_checking = false; // Remove highlight after recording state
            node -> is_focused = true; // Highlight successor
            save_history(9);

            while (temp->left) {
                temp = temp->left;
                temp->is_checking = true; // Highlight path to successor
                save_history(9);
                temp->is_checking = false; // Remove highlight after recording state
            } 

            temp->is_focused = true; 
            save_history(9);// Record state with successor highlighted
            std::swap(node->data, temp->data); // Replace node's data with successor's data
            //node->data = temp->data;
            save_history(9); // Record state after replacement
            temp -> is_focused = false; // Remove highlight from successor
            node -> is_focused = false; // Remove highlight from current node
            save_history(-1); // Record state after removing highlights
            remove(node->right, temp->data);
        }
    }

    if (node) balance(node);
}

/*
    std::vector <std::string> avl_search;
    avl_search.push_back("if (root == NULL) return NULL;");
    avl_search.push_back("if (value < root->data)");
    avl_search.push_back("    return search(root->left, value);");
    avl_search.push_back("else if (value > root->data)");
    avl_search.push_back("    return search(root->right, value);");
    avl_search.push_back("else return root;");
    code_snippets["avl_search"] = avl_search;
*/

bool AVLTree::search(avl_Node* node, int val) {
    if (!node) return false;
    node ->is_checking = true; // Highlight node being checked
    if (val < node -> data) save_history(2);
    if (val > node -> data) save_history(4);

    node ->is_checking = false; // Remove highlight after recording state
    if (val == node->data) {
        node -> is_focused = true; // Highlight found node
        save_history(5);
        node -> is_focused = false; // Remove highlight after recording state
        return true;
    }

    return val < node->data ? search(node->left, val) : search(node->right, val);
}

void AVLTree::draw() {
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
            if (node.exist == 0) continue; // Bỏ qua các node rỗng
            Vector2 pos = { node.x, node.y};
            
            if (node.lefval != -1) {
                Vector2 leftChildPos = { tree[(int)node.lefval].x , tree[(int)node.lefval].y};
                DrawLineEx(pos, leftChildPos, 5.0f, GRAY);
            }

            if (node.rigval != -1) {
                Vector2 rightChildPos = { tree[(int)node.rigval].x , tree[(int)node.rigval].y};
                DrawLineEx(pos, rightChildPos, 5.0f, GRAY);
            }
        }
        
        for (auto& node : tree) {
            if (node.exist == 0) continue; // Bỏ qua các node rỗng
            Vector2 pos = { node.x, node.y};
            
            DrawCircleV(pos, NODE_RADIUS, SKYBLUE);
            Vector2 textSize = MeasureTextEx(font, std::to_string(node.data).c_str(), 30, 1);
            DrawTextEx(font, std::to_string(node.data).c_str(), {pos.x - textSize.x / 2, pos.y - textSize.y / 2}, 30, 1, WHITE);
        }
    } else {
        // Vẽ trạng thái trung gian nếu đang trong quá trình chuyển đổi
        auto& currentTree = history[curent_state];
        auto& nextTree = history[curent_state + 1];

        for (int i = 0; i < currentTree.size(); ++i) {
            Vector2 pos = { currentTree[i].x, currentTree[i].y};
            Vector2 nextPos = { nextTree[i].x, nextTree[i].y};
            Vector2 interpPos = Vector2Lerp(pos, nextPos, progress / time_per_change);

            if (currentTree[i].exist == 0 && nextTree[i].exist == 0) continue; // Bỏ qua các node rỗng

            int leftVal = (int)currentTree[i].lefval;
            int rightVal = (int)currentTree[i].rigval;
            if (leftVal >= 0 && currentTree[leftVal].exist + nextTree[leftVal].exist > 0) {
                Vector2 leftChildPos_cur = { currentTree[leftVal].x , currentTree[leftVal].y};
                Vector2 leftChildPos_next = { nextTree[leftVal].x , nextTree[leftVal].y};
                Vector2 childPos = Vector2Lerp(leftChildPos_cur, leftChildPos_next, progress / time_per_change);
                DrawLineEx(interpPos, childPos, 5.0f, GRAY);
            } 

            if (rightVal >= 0 && currentTree[rightVal].exist + nextTree[rightVal].exist > 0) {
                Vector2 rightChildPos_cur = { currentTree[rightVal].x , currentTree[rightVal].y};
                Vector2 rightChildPos_next = { nextTree[rightVal].x , nextTree[rightVal].y};
                Vector2 childPos = Vector2Lerp(rightChildPos_cur, rightChildPos_next, progress / time_per_change);
                DrawLineEx(interpPos, childPos, 5.0f, GRAY);
            }
        }

        for (int i = 0; i < currentTree.size(); ++i) {
            Vector2 pos = { currentTree[i].x, currentTree[i].y};
            Vector2 nextPos = { nextTree[i].x, nextTree[i].y};
            Vector2 interpPos = Vector2Lerp(pos, nextPos, progress / time_per_change);

            if (currentTree[i].exist == 0 && nextTree[i].exist == 0) continue; // Bỏ qua các node rỗng

            if (currentTree[i].exist == 0 && nextTree[i].exist > 0) {
                DrawCircleV(interpPos, NODE_RADIUS, Fade(SKYBLUE, progress / time_per_change));
            } else if (currentTree[i].exist > 0 && nextTree[i].exist == 0) {
                DrawCircleV(interpPos, NODE_RADIUS, Fade(SKYBLUE, 1 - progress / time_per_change));
            }

            if (nextTree[i].is_checking) {
                DrawRing(interpPos, NODE_RADIUS - 5, NODE_RADIUS + 5, 0, 360, 1, RED);
            }

            if (nextTree[i].is_focused) {
                DrawRing(interpPos, NODE_RADIUS - 5, NODE_RADIUS + 5, 0, 360, 1, GREEN);
            }
            
            DrawCircleV(interpPos, NODE_RADIUS, SKYBLUE);
            Vector2 textSize = MeasureTextEx(font, std::to_string(currentTree[i].data).c_str(), 30, 1);
            DrawTextEx(font, std::to_string(currentTree[i].data).c_str(), {interpPos.x - textSize.x / 2, interpPos.y - textSize.y / 2}, 30, 1, WHITE);
        }
    }
}

void AVLTree::draw_task() {
    progress_duration.draw();
    opp.draw();
    speed.draw();
    assets::Instance().draw_texture("home_icon", {(float)(GetScreenWidth() - 100), 10}, 0.3f);
    if (snippets.empty())
        return;

    highlight_code::Instance().draw(command_type, snippets[std::min(curent_state, (int)snippets.size() - 1)]);
}

std::string AVLTree::Run() {
    tree.assign(1000, {0, 0, 0, 0, 0, 0}); // Initialize tree with empty nodes
    cam.initialize();
    speed.initialize(1350, 800, 100, 50);
    progress_duration.initialize(1000, 800, 200, 0);
    opp.initialize(20, 700);
    
    int screenWidth = 1000;
    int screenHeight = 600; 

    while (!WindowShouldClose()) {
        if (assets::Instance().is_clicked({(float)(GetScreenWidth() - 100), 10}, "home_icon", 0.3f)) {
            break;
        }
        if (IsKeyPressed(KEY_A)) {
            do_insert(GetRandomValue(1, 99));
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
    return "";
}
