#include "raylib.h"
#include <map>
#include <string>
#include <vector>
#include "font.h"
#include <math.h>
#include <iostream>

#pragma once 
class highlight_code {
public:
    std::map<std::string, std::vector<std::string>> code_snippets;
    std::map<std::string, Vector2> position; // key: code snippet key, value: vector of pairs (line index, duration to highlight)
    std::map<std::string, int> width_saving; // key: code snippet key, value: vector of pairs (line index, duration to highlight)
    static highlight_code& Instance() {
        static highlight_code instance;
        return instance;
    }

    void draw(std::string key, int highlight_line = -1) {
        if (code_snippets.find(key) == code_snippets.end()) return;
        Vector2 pos = position[key];
        int len = width_saving[key];
        for (int i = 0; i < code_snippets[key].size(); i++) {
            Color background = {62, 114, 175, 140};
            if (i == highlight_line) {
                background = {62, 114, 175, 230}; // Highlight color
            }
            DrawRectangle(pos.x, pos.y + i * 23, len, 23, background);
            DrawTextEx(font, code_snippets[key][i].c_str(), {pos.x + 10, (float)pos.y + i * 23 + 1.5f}, 20, 1, WHITE);
        }
    }

    int find_snippet(std::string key, std::string code) {
        if (code_snippets.find(key) == code_snippets.end()) return 1000;
        std::cout << "Finding snippet: " << code << " in key: " << key << std::endl;   
        std::cout << code_snippets[key].back() << std::endl; 
        return code_snippets[key].size() - 1;
    }

    void prebuild_position() {
        for (auto x : code_snippets) {
            int height = 23 * x.second.size(), width = 0; // Giả sử mỗi dòng code có chiều cao 30 pixels
            for (int i = 0; i < x.second.size(); i++) {
                // Tính toán vị trí và kích thước của mỗi dòng code
                // Lưu trữ thông tin này để sử dụng khi vẽ
                width = std::max((float)width, MeasureTextEx(font, x.second[i].c_str(), 20, 1).x + 20);
            }
            position[x.first] = Vector2{GetScreenWidth() - (float)width - 20, GetScreenHeight() - (float)height - 200};
            width_saving[x.first] = width;
        }
    }

    void LoadResources() {
        std::vector <std::string> linked_list_insert;
        linked_list_insert.push_back("node cur = head");
        linked_list_insert.push_back("while (cur->next != NULL)");
        linked_list_insert.push_back("    cur = cur->next");
        linked_list_insert.push_back("cur->next = newNode(v)");
        code_snippets["linked_list_insert"] = linked_list_insert;

        std::vector <std::string> linked_list_delete;
        linked_list_delete.push_back("node cur = head");
        linked_list_delete.push_back("while (cur != NULL && cur->data != v)");
        linked_list_delete.push_back("    cur = cur->next");
        linked_list_delete.push_back("if (cur != NULL)");
        linked_list_delete.push_back("    delete cur;");  
        code_snippets["linked_list_delete"] = linked_list_delete;

        std::vector <std::string> linked_list_search;
        linked_list_search.push_back("node cur = head");
        linked_list_search.push_back("while (cur != NULL && cur->data != v)");
        linked_list_search.push_back("    cur = cur->next");
        linked_list_search.push_back("if (cur != NULL) return cur");
        code_snippets["linked_list_search"] = linked_list_search;

        std::vector <std::string> linked_list_update;
        linked_list_update.push_back("node cur = head");
        linked_list_update.push_back("while (cur != NULL && cur->data != oldValue)");
        linked_list_update.push_back("    cur = cur->next");
        linked_list_update.push_back("if (cur != NULL) cur->data = newValue");
        code_snippets["linked_list_update"] = linked_list_update;
        
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

        std::vector <std::string> avl_insert;
        avl_insert.push_back("if (root == NULL) root = newNode(value)");
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
        avl_update.push_back("if (target != NULL) {");
        avl_update.push_back("    deleteNode(root, oldValue);");
        avl_update.push_back("    root = insert(root, newValue);");
        avl_update.push_back("}");
        code_snippets["avl_update"] = avl_update;

        std::vector <std::string> trie_insert;
        trie_insert.push_back("node cur = root");
        trie_insert.push_back("for (char c : word)");
        trie_insert.push_back("    if (cur->children[c - 'a'] == NULL)");
        trie_insert.push_back("        cur->children[c - 'a'] = newNode();");
        trie_insert.push_back("    cur = cur->children[c - 'a'];");
        trie_insert.push_back("cur->isEndOfWord = true;");
        code_snippets["trie_insert"] = trie_insert;

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

        std::vector <std::string> kruskal;
        kruskal.push_back("sort edges by weight");
        kruskal.push_back("for (edge in edges)");
        kruskal.push_back("    if (find(edge.u) != find(edge.v)) {");
        kruskal.push_back("        union(edge.u, edge.v);");    
        kruskal.push_back("        add edge to MST;");
        kruskal.push_back("    else erase edge from consideration");
        code_snippets["kruskal"] = kruskal;

        std::vector <std::string> dijkstra;
        dijkstra.push_back("while priority queue is not empty");
        dijkstra.push_back("    u = vertex with smallest distance");
        dijkstra.push_back("    for each neighbor v of u");
        dijkstra.push_back("        dist[v] = min(dist[v], dist[u] + weight(u, v))");
        dijkstra.push_back("        update priority queue");
        code_snippets["dijkstra"] = dijkstra;
    }

    // Lấy texture theo key mà không cần load lại
    Texture2D Get(std::string key) {
        return textures[key];
    }

    void draw_texture(std::string key, Vector2 position, float scale = 1.0f) {
        if (textures.find(key) != textures.end()) {
            Texture2D tex = textures[key];
            DrawTextureEx(tex, position, 0.0f, scale, WHITE);
        }
    }

    // Giải phóng bộ nhớ khi tắt app
    void UnloadResources() {
        for (auto& pair : textures) {
            UnloadTexture(pair.second);
        }
    }

private:
    highlight_code() {} // Constructor private để chặn tạo mới
    std::map<std::string, Texture2D> textures;
};