#include "trie.h"    

void Trie::GetContour(int nodeIdx, float modSum, std::map<int, float>& contour, bool isLeft) {
    float currX = tree[nodeIdx].x + modSum;
    
    if (contour.find(tree[nodeIdx].depth) == contour.end()) {
        contour[tree[nodeIdx].depth] = currX;
    } else {
        if (isLeft) contour[tree[nodeIdx].depth] = std::min(contour[tree[nodeIdx].depth], currX);
        else contour[tree[nodeIdx].depth] = std::max(contour[tree[nodeIdx].depth], currX);
    }

    for (int childId : tree[nodeIdx].children) {
        if (tree[childId].cnt > 0) {
            GetContour(childId, modSum + tree[nodeIdx].mod, contour, isLeft);
        }
    }
}

bool Trie::is_empty(int node) {
    for (int i = 0; i < tree[node].children.size(); ++i) {
        if (tree[tree[node].children[i]].cnt > 0) return false;
    }
    return true;
}

void Trie::ResolveConflicts(int nodeIdx) {
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

void Trie::FirstPass(int nodeIdx) {
    auto& node = tree[nodeIdx];
    if (is_empty(nodeIdx)) {
        int leftSibIdx = -1;
        int pIdx = node.parentIdx;
        if (pIdx != -1) {
            for (int i = 1; i < tree[pIdx].children.size(); ++i) {
                if (tree[pIdx].children[i] == nodeIdx) {
                    for (int j = i - 1; j >= 0; --j) {
                        if (tree[tree[pIdx].children[j]].cnt > 0) {
                            leftSibIdx = tree[pIdx].children[j];
                            break;
                        }
                    }
                }
            }
        }
        node.x = (leftSibIdx != -1) ? tree[leftSibIdx].x + SIBLING_SEP : 0;
    } else {
        for (int childId : node.children) if (tree[childId].cnt > 0) FirstPass(childId);
        float tot = 0;
        for (int i = 0; i < node.children.size(); ++i) {
            if (tree[node.children[i]].cnt > 0) {tot += tree[node.children[i]].x; break;}
        }

        for (int i = node.children.size() - 1; i >= 0; --i) {
            if (tree[node.children[i]].cnt > 0) {tot += tree[node.children[i]].x; break;}
        }

        float midpoint = tot / 2.0f;
        int leftSibIdx = -1;
        int pIdx = node.parentIdx;

        if (pIdx != -1) {
            for (int i = 1; i < tree[pIdx].children.size(); ++i) {
                if (tree[pIdx].children[i] == nodeIdx) {
                    for (int j = i - 1; j >= 0; --j) {
                        if (tree[tree[pIdx].children[j]].cnt > 0) {
                            leftSibIdx = tree[pIdx].children[j];
                            break;
                        }
                    }
                }
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

std::string Trie::Run() {
    SetTargetFPS(60);
    cam.initialize();
    opp.initialize(20, 700, 1);
    speed.initialize(1350, 800, 100, 50);
    progress_duration.initialize(1000, 800, 200, 0);
    
    int screenWidth = 1000;
    int screenHeight = 600; 

    while (!WindowShouldClose()) {
        if (assets::Instance().is_clicked({(float)(GetScreenWidth() - 100), 10}, "home_icon", 0.3f)) {
            break;
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

/*
        std::vector <std::string> trie_delete;
        trie_delete.push_back("node cur = root");
        trie_delete.push_back("for (char c : word)");
        trie_delete.push_back("    if (cur->children[c - 'a'] == NULL) return;");
        trie_delete.push_back("    cur = cur->children[c - 'a'];");
        trie_delete.push_back("cur->isEndOfWord = false;");
        code_snippets["trie_delete"] = trie_delete;

        std::vector <std::string> trie_search;
        trie_search.push_back("node cur = root");
        trie_search.push_back("for (char c : word)");
        trie_search.push_back("    if (cur->children[c - 'a'] == NULL) return false;");
        trie_search.push_back("    cur = cur->children[c - 'a'];");
        trie_search.push_back("return cur->isEndOfWord;");
        code_snippets["trie_search"] = trie_search;

        std::vector <std::string> trie_update;
        trie_update.push_back("node target = search(root, oldWord);");
        trie_update.push_back("if (target != NULL)");
        trie_update.push_back("    deleteNode(root, oldWord);");
        trie_update.push_back("    insert(newWord);");
        code_snippets["trie_update"] = trie_update;

        std::vector <std::string> trie_insert;
        trie_insert.push_back("node cur = root");
        trie_insert.push_back("for (char c : word)");
        trie_insert.push_back("    if (cur->children[c - 'a'] == NULL)");
        trie_insert.push_back("        cur->children[c - 'a'] = newNode();");
        trie_insert.push_back("    cur = cur->children[c - 'a'];");
        trie_insert.push_back("cur->isEndOfWord = true;");
        code_snippets["trie_insert"] = trie_insert;
*/

void Trie::update() {
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

/*
    std::vector <std::string> trie_search;
    trie_search.push_back("node cur = root");
    trie_search.push_back("for (char c : word)");
    trie_search.push_back("    if (cur->children[c - 'a'] == NULL) return false;");
    trie_search.push_back("    cur = cur->children[c - 'a'];");
    trie_search.push_back("return cur->isEndOfWord;");
    code_snippets["trie_search"] = trie_search;
*/

void Trie::do_search(std::string word) {
    history.clear();
    curent_state = 0;
    progress = 0;
    history.push_back(tree);
    snippets.clear();
    snippets.push_back(0);

    int curr = rootIdx;
    for (char c : word) {
        int next = -1;
        for (int childId : tree[curr].children) {
            if (tree[childId].value == c) {
                next = childId;  
                tree[childId].is_checking = true; 
                history.push_back(tree); 
                snippets.push_back(3);
                tree[childId].is_checking = false; // Tắt hiệu ứng kiểm tra sau khi đã lưu trạng thái
                break;
            }
        }
        if (next == -1) {
            history.push_back(tree);
            snippets.push_back(2);
            return;
        }
        curr = next;
    }

    if (tree[curr].isEndOfWord) {
        tree[curr].is_focused = true; // Hiệu ứng tập trung vào node cuối nếu tìm thấy từ
    }

    history.push_back(tree);
    snippets.push_back(4);

    tree[curr].is_focused = false; // Tắt hiệu ứng tập trung sau khi đã lưu trạng thái
    // Có thể thêm hiệu ứng nếu tree[curr].isEndOfWord == true
}

/*
    std::vector <std::string> trie_delete;
    trie_delete.push_back("node cur = root");
    trie_delete.push_back("for (char c : word)");
    trie_delete.push_back("    if (cur->children[c - 'a'] == NULL) return;");
    trie_delete.push_back("    cur = cur->children[c - 'a'];");
    trie_delete.push_back("cur->isEndOfWord = false;");
    code_snippets["trie_delete"] = trie_delete;
*/

void Trie::do_delete_node(std::string word) {
    curent_state = 0;
    progress = 0;
    history.clear();
    snippets.clear();
    snippets.push_back(0);

    int add = search(word); // Nếu từ không tồn tại thì không làm gì
    add *= -1; // Chuyển sang -1 để giảm cnt nếu đang xóa node
    // Xóa node đánh dấu kết thúc từ, không xóa node thực tế để tránh phức tạp
    int curr = rootIdx;
    tree[curr].cnt += add; // Giảm cnt của root nếu đang xóa node

    std::vector<int> path; // Lưu đường đi để có thể cập nhật layout sau khi xóa
    path.push_back(curr);

    for (char c : word) {
        int next = -1;
        for (int childId : tree[curr].children) {
            if (tree[childId].value == c) {
                next = childId;  
                tree[childId].is_checking = true;
                //UpdateLayout();
                history.push_back(tree);
                snippets.push_back(3); 
                tree[childId].is_checking = false; // Tắt hiệu ứng kiểm tra sau khi đã lưu trạng thái
                break;
            }
        }

        if (next == -1) {
            history.push_back(tree);
            snippets.push_back(-1);
            return;
        }

        curr = next;
        path.push_back(curr);
    }

    bool checking = 0;
    if (search(word)) {tree[curr].isEndOfWord = 0; snippets.push_back(4); checking = 1;} // Nếu từ tồn tại thì xóa đánh dấu kết thúc từ, nếu không tồn tại thì không làm gì
    UpdateLayout(); // Nếu từ tồn tại thì xóa đánh dấu kết thúc từ, nếu không tồn tại thì không làm gì
    history.push_back(tree);
    if (!checking)
        snippets.push_back(-1); // Nếu từ tồn tại thì thêm snippet xóa node, nếu không tồn tại thì không làm gì

    if (add == -1) { // Nếu đang xóa node thì mới cần cập nhật layout
        for (int i = path.size() - 1; i >= 0; --i) {
            tree[path[i]].cnt--; // Cập nhật cnt trên đường đi
            if (tree[path[i]].cnt == 0) {
                UpdateLayout(); // Cập nhật layout sau khi cnt về 0 để node này biến mất khỏi layout
                history.push_back(tree);
                snippets.push_back(-1); // Thêm snippet xóa node
            }// Nếu cnt vẫn còn > 0 thì dừng lại vì node này vẫn còn tồn tại 
        }
    }
}

void Trie::delete_node(std::string word) {
    if (search(word) == false) return; 
    int curr = rootIdx;

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

void Trie::do_insert(std::string word) {
    curent_state = 0;
    progress = 0;
    history.clear();
    history.push_back(tree);
    insert(word);
    UpdateLayout();
    history.push_back(tree);
}

void Trie::draw_task() {
    progress_duration.draw();
    opp.draw();
    speed.draw();
    assets::Instance().draw_texture("home_icon", {(float)(GetScreenWidth() - 100), 10}, 0.3f);
    if (snippets.size() == 0) return;

    if (opp.command == "Insert") {
        highlight_code::Instance().draw("trie_insert", snippets[std::min(curent_state, (int)snippets.size() - 1)]);
    } else if (opp.command == "Delete") {
        highlight_code::Instance().draw("trie_delete", snippets[std::min(curent_state, (int)snippets.size() - 1)]);
    } else if (opp.command == "Search") {
        highlight_code::Instance().draw("trie_search", snippets[std::min(curent_state, (int)snippets.size() - 1)]);
    }
}

bool Trie::search(std::string word) {
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

/*
    std::vector <std::string> trie_insert;
    trie_insert.push_back("node cur = root");
    trie_insert.push_back("for (char c : word)");
    trie_insert.push_back("    if (cur->children[c - 'a'] == NULL)");
    trie_insert.push_back("        cur->children[c - 'a'] = newNode();");
    trie_insert.push_back("    cur = cur->children[c - 'a'];");
    trie_insert.push_back("cur->isEndOfWord = true;");
    code_snippets["trie_insert"] = trie_insert;
*/

void Trie::insert(std::string word) {
    history.clear();
    history.push_back(tree);
    snippets.clear();
    snippets.push_back(0);

    int curr = rootIdx, add = !search(word); // Nếu từ đã tồn tại thì chỉ thêm hiệu ứng kiểm tra, không thêm node mới
    tree[curr].cnt += add; // Tăng cnt của root nếu đang thêm node mới

    for (char c : word) {
        int next = -1;
        for (int childId : tree[curr].children) {
            if (tree[childId].value == c) {
                next = childId;  
                tree[childId].is_checking = true;
                tree[next].cnt += add; // Chỉ tăng cnt nếu đang thêm node mới 
                history.push_back(tree); 
                snippets.push_back(4);
                tree[childId].is_checking = false; // Tắt hiệu ứng kiểm tra sau khi đã lưu trạng thái
                break;
            }
        }

        if (next == -1) { 
            int newId = (int)tree.size();
            tree.emplace_back(c, newId, tree[curr].depth + 1, curr);
            tree[curr].children.push_back(newId);
            curr = newId;
            tree[curr].cnt = add; // Node mới có cnt = 1
            UpdateLayout();
            history.push_back(tree);
            snippets.push_back(3);
            tree[curr].is_checking = true;
            history.push_back(tree);
            snippets.push_back(3);
            tree[curr].is_checking = false;
        } else curr = next;
    }

    tree[curr].isEndOfWord = true;
    history.push_back(tree);
    snippets.push_back(5);
}

void Trie::draw() {
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
            if (node.cnt == 0) continue; // Bỏ qua các node rỗng
            Vector2 pos = { node.x, node.y};
            for (int childId : node.children) {
                if (tree[childId].cnt == 0) continue; // Bỏ qua các node rỗng
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

            if (currentTree[i].cnt == 0 && nextTree[i].cnt == 0) continue; // Bỏ qua các node rỗng

            if (currentTree[i].cnt == 0 && nextTree[i].cnt > 0) {
                DrawCircleV(interpPos, NODE_RADIUS, Fade(SKYBLUE, progress / time_per_change));
            } else if (currentTree[i].cnt > 0 && nextTree[i].cnt == 0) {
                DrawCircleV(interpPos, NODE_RADIUS, Fade(SKYBLUE, 1 - progress / time_per_change));
            }

            if (nextTree[i].is_checking) {
                DrawRing(interpPos, NODE_RADIUS - 5, NODE_RADIUS + 5, 0, 360, 1, RED);
            }

            for (int childId : currentTree[i].children) {
                Vector2 childPos = { currentTree[childId].x , currentTree[childId].y};
                Vector2 nextChildPos = { nextTree[childId].x , nextTree[childId].y};
                if (currentTree[childId].cnt + nextTree[childId].cnt > 0) {
                    childPos = Vector2Lerp(childPos, nextChildPos, progress / time_per_change);
                    DrawLineEx(interpPos, childPos, 5.0f, GRAY);
                } 
            }
            
            DrawCircleV(interpPos, NODE_RADIUS, currentTree[i].isEndOfWord ? PINK : SKYBLUE);
            Vector2 textSize = MeasureTextEx(GetFontDefault(), std::string(1, currentTree[i].value).c_str(), 30, 1);
            DrawTextEx(font, std::string(1, currentTree[i].value).c_str(), {interpPos.x - textSize.x / 2, interpPos.y - textSize.y / 2}, 30, 1, WHITE);
        }

        for (int i = currentTree.size(); i < nextTree.size(); ++i) {
            if (nextTree[i].cnt == 0) continue; // Bỏ qua các node rỗng
            Vector2 pos = { nextTree[i].x, nextTree[i].y - 100};
            Vector2 nextPos = { nextTree[i].x, nextTree[i].y};
            Vector2 interpPos = Vector2Lerp(pos, nextPos, progress / time_per_change);
            DrawCircleV(interpPos, NODE_RADIUS, nextTree[i].isEndOfWord ? PINK : SKYBLUE);
            Vector2 textSize = MeasureTextEx(GetFontDefault(), std::string(1, nextTree[i].value).c_str(), 30, 1);
            DrawTextEx(font, std::string(1, nextTree[i].value).c_str(), {interpPos.x - textSize.x / 2, interpPos.y - textSize.y / 2}, 30, 1, WHITE);
            if (nextTree[i].is_checking) {
                DrawRing(interpPos, NODE_RADIUS - 5, NODE_RADIUS + 5, 0, 360, 1, RED);
            }
        }
    }
}

void Trie::UpdateLayout() {
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

void Trie::SecondPass(int nodeIdx, float cumMod, float& xMin, Vector2 offset) {
    auto& node = tree[nodeIdx];
    node.x += cumMod + offset.x;
    node.y = node.depth * LEVEL_SEP + offset.y;

    if (node.x < xMin) xMin = node.x;
    for (int childId : node.children) if (tree[childId].cnt > 0) SecondPass(childId, cumMod + node.mod, xMin, offset);
}