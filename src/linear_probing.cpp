#include "linear_probing.h"

Vector2 LinearProbingVisualizer::get_pos(int i, int j) {
    Vector2 A = list[i][j].start_pos;
    Vector2 B = list[i][j].target_pos;
    return { A.x +  (progress / time_per_change) * (B.x - A.x), A.y +  (progress / time_per_change) * (B.y - A.y) };
}

float LinearProbingVisualizer::get_alpha(int i, int j) {
    return list[i][j].alpha_start + (1 - list[i][j].alpha_start) * (progress / time_per_change);
}

void LinearProbingVisualizer::DrawLinkedList(int id) {
    // Implement the insert operation for linear probing hash table
    if (!list[id].empty()) {
        Color arrowColor = Fade(GRAY, 1.0f);    
        Vector2 start = { id * 110 + 100, 400 };
        Vector2 end = { id * 110 + 100, 520};
        DrawLineEx(start, end, 2.0f, arrowColor);
        DrawTriangle({end.x + 5, end.y - 5}, {end.x - 5, end.y - 5}, end, arrowColor);
    }

    for (int i = 0; i < list[id].size(); i++) {
        const Node &node = list[id][i];
        Color mainColor = Fade(SKYBLUE, get_alpha(id, i));
        Color borderColor = Fade(BLUE, get_alpha(id, i));
        Color textColor = Fade(DARKGRAY, get_alpha(id, i));
        Color arrowColor = Fade(GRAY, get_alpha(id, i));

        Vector2 real_pos = get_pos(id, i);
        if (i < list[id].size() - 1) {
            Vector2 start = { real_pos.x, real_pos.y + 40};
            Vector2 end = { get_pos(id, i + 1).x, get_pos(id, i + 1).y - 40};
            DrawLineEx(start, end, 2.0f, arrowColor);
            DrawTriangle({end.x + 5, end.y - 5}, {end.x - 5, end.y - 5}, end, arrowColor);
        }
        Vector2 center = { real_pos.x, real_pos.y};
        DrawCircleV(center, node.size / 2, mainColor);
        if (node.is_checking) {
            DrawRing({(float)center.x, (float)center.y}, node.size / 2 - 2, node.size / 2 + 1, 0, 360, 1, RED);
        }
        
        if (node.is_focused) {
            DrawRing({(float)center.x, (float)center.y}, node.size / 2 - 2, node.size / 2 + 1, 0, 360, 1, GREEN);
        }

        std::string text = std::to_string(node.data);
        int textWidth = MeasureText(text.c_str(), 20);
        DrawTextEx(font ,text.c_str(), {(float)real_pos.x - textWidth / 2, (float)real_pos.y - 10}, 20, 1, textColor);
    }
}

void LinearProbingVisualizer::draw() {
    Color boxColor = { 184, 242, 230, 255 }; 
    Color borderColor = { 106, 76, 147, 200 }; 
    float cornerRoundness = 0.2f; 

    for (int i = 0; i < 10; i++) {
        Rectangle rec = { (float)(i * 110 + 50), 300.0f, 100.0f, 100.0f };
        DrawRectangleRoundedLines(rec, cornerRoundness, 20, borderColor);
        DrawRectangleRounded(rec, cornerRoundness, 20, boxColor);
        std::string text = std::to_string(i);
        int fontSize = 30;
        int textWidth = MeasureText(text.c_str(), fontSize);
        DrawText(text.c_str(), rec.x + (100 - textWidth) / 2, rec.y + (100 - fontSize) / 2, fontSize, borderColor);
        DrawLinkedList(i);
    }

    //if (history.empty()) return;

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
        list = history[curent_state];
    }

    if (speed.is_changed) {
        time_per_change = 0.05f + (100 - speed.value) / 50;
        speed.is_changed = false;
    }
}

void LinearProbingVisualizer::draw_task() {
    progress_duration.draw();
    opp.draw();
    speed.draw();
    assets::Instance().draw_texture("home_icon", {(float)(GetScreenWidth() - 100), 10}, 0.3f);
    if (snippets.empty()) return;
    if (opp.command == "Insert") {
        highlight_code::Instance().draw("chaining_insert", snippets[std::min(curent_state, (int)snippets.size() - 1)]);
    } else if (opp.command == "Delete") {
        highlight_code::Instance().draw("chaining_delete", snippets[std::min(curent_state, (int)snippets.size() - 1)]);
    } else if (opp.command == "Search") {
        highlight_code::Instance().draw("chaining_search", snippets[std::min(curent_state, (int)snippets.size() - 1)]);
    }
}

/*
        std::vector <std::string> chaining_insert;
        chaining_insert.push_back("int idx = value % table_size");
        chaining_insert.push_back("node cur = table[idx]");
        chaining_insert.push_back("while (cur->next != NULL)");
        chaining_insert.push_back("    cur = cur->next");
        chaining_insert.push_back("cur->next = newNode(value)");
        code_snippets["chaining_insert"] = chaining_insert;

        std::vector <std::string> chaining_delete;
        chaining_delete.push_back("int idx = value % table_size");
        chaining_delete.push_back("node cur = table[idx]");
        chaining_delete.push_back("while (cur != NULL && cur->data != value)");
        chaining_delete.push_back("    cur = cur->next");
        chaining_delete.push_back("if (cur != NULL) delete cur" );
        code_snippets["chaining_delete"] = chaining_delete;

        std::vector <std::string> chaining_search;
        chaining_search.push_back("int idx = value % table_size");
        chaining_search.push_back("node cur = table[idx]");
        chaining_search.push_back("while (cur != NULL && cur->data != value)");
        chaining_search.push_back("    cur = cur->next");
        chaining_search.push_back("if (cur != NULL) return cur");
        code_snippets["chaining_search"] = chaining_search;

        std::vector <std::string> chaining_update;
        chaining_update.push_back("int idx = oldvalue % table_size");
        chaining_update.push_back("node cur = table[idx]");
        chaining_update.push_back("while (cur != NULL && cur->data != oldvalue)");
        chaining_update.push_back("    cur = cur->next");
        chaining_update.push_back("if (cur != NULL) delete cur");
        chaining_update.push_back("int newIdx = newValue % table_size");
        chaining_update.push_back("cur = table[newIdx]");
        chaining_update.push_back("while (cur->next != NULL)");
        chaining_update.push_back("    cur = cur->next");
        chaining_update.push_back("cur->next = newNode(newValue)");
        code_snippets["chaining_update"] = chaining_update;
*/

/*
        std::vector <std::string> chaining_insert;
        chaining_insert.push_back("int idx = value % table_size");
        chaining_insert.push_back("node cur = table[idx]");
        chaining_insert.push_back("while (cur->next != NULL)");
        chaining_insert.push_back("    cur = cur->next");
        chaining_insert.push_back("cur->next = newNode(value)");
        code_snippets["chaining_insert"] = chaining_insert;
*/

void LinearProbingVisualizer::insert(int value) {
    curent_state = 0;
    history.clear();
    history.push_back(list);
    snippets.clear();
    snippets.push_back(1);
    int id = value % 10;

    for (int i = 0; i < list[id].size(); i++) {
        list[id][i].is_checking = true;
        history.push_back(list);
        snippets.push_back(3);
        list[id][i].is_checking = false;
    }

    if (!list[id].empty()) {
        list[id].back().is_checking = false;
    }

    history.push_back(list);
    Node newNode;
    newNode.data = value;
    newNode.alpha_start = 0.0f;
    newNode.alpha_end = 1.0f;
    if (list[id].empty()) {
        newNode.start_pos = { (float)id * 110 + 100, 400 };
        newNode.target_pos = { (float)id * 110 + 100, 560 };
    } else {
        newNode.start_pos = { (float)id * 110 + 100, list[id].back().target_pos.y };
        newNode.target_pos = { (float)id * 110 + 100,  list[id].back().target_pos.y + 160 };
    }

    list[id].push_back(newNode);
    snippets.push_back(4);
    history.push_back(list);
    list[id].back().start_pos = list[id].back().target_pos;
    list[id].back().alpha_start = 1.0f;
    history.push_back(list);
    snippets.push_back(-1);
    list = history[0];
}

/*
        std::vector <std::string> chaining_delete;
        chaining_delete.push_back("int idx = value % table_size");
        chaining_delete.push_back("node cur = table[idx]");
        chaining_delete.push_back("while (cur != NULL && cur->data != value)");
        chaining_delete.push_back("    cur = cur->next");
        chaining_delete.push_back("if (cur != NULL) delete cur" );
        code_snippets["chaining_delete"] = chaining_delete;
*/

void LinearProbingVisualizer::delete_node(int value) {
    if (list.empty()) return;  
    curent_state = 0;
    history.clear();
    history.push_back(list);
    snippets.clear();
    snippets.push_back(1);

    int id = value % 10, choose_index = -1;
    for (int i = 0; i < list[id].size(); i++) {
        list[id][i].is_checking = true;
        history.push_back(list);
        list[id][i].is_checking = false;
        snippets.push_back(3);

        if (list[id][i].data == value) {
            choose_index = i;
            list[id].erase(list[id].begin() + i);
            for (int j = i; j < list[id].size(); j++) {
                list[id][j].target_pos.y -= 160;
            }
            break;
        }
    }

    if (choose_index == -1) {
        list[id].back().is_checking = false;
        snippets.push_back(-1);
        history.push_back(list);
        return;
    }

    history.push_back(list);    
    for (int i = choose_index; i < list[id].size(); i++) {
        list[id][i].start_pos.y -= 160;
    }

    snippets.push_back(4);

    history.push_back(list);
    list = history[0];
    snippets.push_back(-1);
}

/*
    std::vector <std::string> chaining_search;
    chaining_search.push_back("int idx = value % table_size");
    chaining_search.push_back("node cur = table[idx]");
    chaining_search.push_back("while (cur != NULL && cur->data != value)");
    chaining_search.push_back("    cur = cur->next");
    chaining_search.push_back("if (cur != NULL) return cur");
    code_snippets["chaining_search"] = chaining_search;
*/

void LinearProbingVisualizer::search(int value) {
    if (list.empty()) return;
    curent_state = 0;
    history.clear();
    history.push_back(list);
    snippets.clear();
    snippets.push_back(1);

    int id = value % 10;
    bool is_searched = false;
    for (int i = 0; i < list[id].size(); i++) {
        list[id][i].is_checking = true;
        history.push_back(list);
        list[id][i].is_checking = false;

        snippets.push_back(3);

        if (list[id][i].data == value) {
            list[id][i].is_focused = true;
            history.push_back(list);
            snippets.push_back(4);
            is_searched = true;
        }
    }

    if (is_searched) {
        for (auto &node : list[id]) {
            node.is_focused = false;
        }
    }

    history.push_back(list);
    list = history[0];
    snippets.push_back(-1);
}

void LinearProbingVisualizer::update() {
    if (opp.is_pending) {
        if (opp.command == "Insert") {
            insert(stoi(opp.str_value));
        } else if (opp.command == "Delete") {
            delete_node(stoi(opp.str_value));
        } else if (opp.command == "Search") {
            search(stoi(opp.str_value));
        }
        opp.is_pending = false;
    }
}

std::string LinearProbingVisualizer::run() {
    cam.initialize();
    opp.initialize(20, 700);
    list.resize(10);
    for (int i = 0; i < 10; i++) {
        list[i].clear();
    }

    int screenWidth = 1000;
    int screenHeight = 600; 

    speed.initialize(1350, 800, 100, 50);
    progress_duration.initialize(1000, 800, 200, 0);
    
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
                list = history[++curent_state];
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
    return "none";
}
