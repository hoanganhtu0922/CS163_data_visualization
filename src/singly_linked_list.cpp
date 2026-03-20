#include "singly_linked_list.h"
#include <iostream>
Vector2 SinglyLinkedList::get_pos(int i) {
    Vector2 A = list[i].start_pos;
    Vector2 B = list[i].target_pos;
    return { A.x +  (progress / time_per_change) * (B.x - A.x), A.y +  (progress / time_per_change) * (B.y - A.y) };
}

float SinglyLinkedList::get_alpha(int i) {
    return list[i].alpha_start + (1 - list[i].alpha_start) * (progress / time_per_change);
}

void SinglyLinkedList::insert(int value) {
    curent_state = 0;
    history.clear();
    history.push_back(list);
    for (int i = 0; i < list.size(); i++) {
        list[i].is_checking = true;
        if (i) {
            list[i-1].is_checking = false;
        }
        history.push_back(list);
    }

    Node newNode;
    newNode.data = value;
    newNode.alpha_start = 0.0f;
    newNode.alpha_end = 1.0f;
    if (list.empty()) {
        newNode.start_pos = { -100, 300 };
        newNode.target_pos = { 50, 300 };
    } else {
        newNode.start_pos = { list.back().target_pos.x, 300 };
        newNode.target_pos = { list.back().target_pos.x + 160, 300 };
    }

    if (list.size()) {
        list.back().is_checking = false;
    }

    list.push_back(newNode);
    history.push_back(list);
    list.pop_back();
    newNode.start_pos = newNode.target_pos;
    newNode.alpha_start = 1.0f;
    list.push_back(newNode);
    history.push_back(list);
    list = history[0];
}


void SinglyLinkedList::delete_node(int value) {
    if (list.empty()) return;  
    curent_state = 0;
    history.clear();
    history.push_back(list);
    int id = -1;
    for (int i = 0; i < list.size(); i++) {
        list[i].is_checking = true;
        history.push_back(list);
        list[i].is_checking = false;

        if (list[i].data == value) {
            id = i;
            list.erase(list.begin() + i);
            for (int j = i; j < list.size(); j++) {
                list[j].target_pos.x -= 160;
            }
            break;
        }
    }

    if (id == -1) {
        list.back().is_checking = false;
        history.push_back(list);
        return;
    }

    history.push_back(list);    
    for (int i = id; i < list.size(); i++) {
        list[i].start_pos.x -= 160;
    }

    history.push_back(list);
    list = history[0];
}

void SinglyLinkedList::search(int value) {
    if (list.empty()) return;
    curent_state = 0;
    history.clear();
    history.push_back(list);
    bool is_searched = false;
    for (int i = 0; i < list.size(); i++) {
        list[i].is_checking = true;
        history.push_back(list);
        list[i].is_checking = false;

        if (list[i].data == value) {
            list[i].is_focused = true;
            history.push_back(list);
            is_searched = true;
        }
    }

    if (is_searched) {
        for (auto &node : list) {
            node.is_focused = false;
        }
    }

    history.push_back(list);
    list = history[0];
}

void SinglyLinkedList::draw() {
    //if (history.empty()) return;

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
        list = history[curent_state];
    }

    if (speed.is_changed) {
        time_per_change = 0.05f + (100 - speed.value) / 50;
        speed.is_changed = false;
    }

    for (int i = 0; i < list.size(); i++) {
        const Node &node = list[i];
        Color mainColor = Fade(CLITERAL(Color){0, 180, 216, 255 }, get_alpha(i));
        Color borderColor = Fade(CLITERAL(Color){ 0, 0, 255, 255 }, get_alpha(i));
        Color textColor = Fade(CLITERAL(Color){ 80, 80, 80, 255 }, get_alpha(i));
        Color arrowColor = Fade(CLITERAL(Color){ 128, 128, 128, 255 }, get_alpha(i));

        Vector2 real_pos = get_pos(i);
        if (i < list.size() - 1) {
            Vector2 start = { real_pos.x + 40, real_pos.y };
            Vector2 end = { get_pos(i + 1).x - 40, get_pos(i + 1).y };
            DrawLineEx(start, end, 2.0f, arrowColor);
            DrawTriangle({end.x - 10, end.y - 5}, {end.x - 10, end.y + 5}, end, arrowColor);
        }
        Vector2 center = { real_pos.x, real_pos.y};
        DrawCircleV(center, node.size / 2, mainColor);
        if (node.is_checking) {
            DrawRing({(float)center.x, (float)center.y}, node.size / 2 - 2, node.size / 2 + 1, 0, 360, 1, RED);
        }
        
        if (node.is_focused) {
            DrawRing({(float)center.x, (float)center.y}, node.size / 2 - 2, node.size / 2 + 1, 0, 360, 1, GREEN);
        }
        //else {
            //DrawRing({(float)center.x, (float)center.y}, node.size / 2 - 1, node.size / 2 + 1, 0, 360, 1, borderColor);
        //}

        std::string text = std::to_string(node.data);
        int textWidth = MeasureText(text.c_str(), 20);
        DrawTextEx(font ,text.c_str(), {(float)real_pos.x - textWidth / 2, (float)real_pos.y - 10}, 20, 1, textColor);
    }
}

void SinglyLinkedList::update() {
    if (opp.is_pending) {
        if (opp.command == "Insert") {
            insert(opp.value);
        } else if (opp.command == "Delete") {
            delete_node(opp.value);
        } else if (opp.command == "Search") {
            search(opp.value);
        }
        opp.is_pending = false;
    }
}

void SinglyLinkedList::draw_task() {
    progress_duration.draw();
    opp.draw();
    speed.draw();
    list_data.draw();
}

std::string SinglyLinkedList::run() {
    cam.initialize();
    opp.initialize(20, 700);
    speed.initialize(850, 800, 100, 50);
    list_data.init(20, 50);
    list_data.choose = "Linked List";
    list_data.focused = 1;
    int screenWidth = 1000;
    int screenHeight = 600; 

    progress_duration.initialize(550, 800, 200, 0);

    while (!WindowShouldClose()) {
        if (list_data.choose != "Linked List") {
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
