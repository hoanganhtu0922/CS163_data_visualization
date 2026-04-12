#include "textbox.h"

void text_box::initialize(int x, int y, int width, int height, std::string initialText, bool _accepting_long) {
    bounds = { (float)x, (float)y, (float)width, (float)height };
    text = initialText;
    num = ""; 
    cursorIndex = 0;
    scrollOffset = 0;
    accepting_long = _accepting_long;
}

void text_box::draw() {
    if (!appear) return;   

    // Vẽ nhãn phía trên
    Vector2 textSize = MeasureTextEx(font, text.c_str(), 20, 1);
    DrawTextEx(font, text.c_str(), { bounds.x + bounds.width/2 - textSize.x/2, bounds.y - 30 }, 20, 1, DARKGRAY);

    // Vẽ khung nền
    DrawRectangleRec(bounds, LIGHTGRAY);
    assets::Instance().draw_texture("random_icon", { bounds.x + 1 + bounds.width, bounds.y}, 0.14f);

    if (focused)
        DrawRectangleLinesEx(bounds, 2, BLUE);
    else
        DrawRectangleLinesEx(bounds, 1, DARKGRAY);

    BeginScissorMode((int)bounds.x + 5, (int)bounds.y, (int)bounds.width - 10, (int)bounds.height);
        
        float startX = bounds.x + 10 - scrollOffset;
        float posY = bounds.y + bounds.height/2 - 10; // Căn giữa theo chiều dọc

        // Vẽ nội dung text
        DrawTextEx(font, num.c_str(), { startX, posY }, 20, 1, BLACK);

        // Vẽ con trỏ nhấp nháy tại vị trí cursorIndex
        if (focused && cursorVisible) {
            std::string textBeforeCursor = num.substr(0, cursorIndex);
            Vector2 sizeBefore = MeasureTextEx(font, textBeforeCursor.c_str(), 20, 1);
            DrawTextEx(font, "|", { startX + sizeBefore.x - 2, posY }, 20, 1, RED); 
        }

    EndScissorMode();
}

void text_box::update() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        focused = CheckCollisionPointRec(GetMousePosition(), bounds);
    }

    if (accepting_long)
        max_length = 1000;
    else
        max_length = !isabc ? 3 : 30;

    // Xử lý nút Random
    if (assets::Instance().is_clicked({ bounds.x + 1 + bounds.width, bounds.y}, "random_icon", 0.14f)) {
        if (isabc) {
            int len = GetRandomValue(5, 15);
            num = "";
            for (int i = 0; i < len; i++) num.push_back('a' + GetRandomValue(0, 25));
        } else {
            num = std::to_string(GetRandomValue(1, 999));
        }
        cursorIndex = num.length(); // Đưa con trỏ về cuối sau khi random
    }

    if (!focused) return;

    // 1. Di chuyển con trỏ bằng mũi tên
    if (IsKeyPressed(KEY_LEFT) && cursorIndex > 0) cursorIndex--;
    if (IsKeyPressed(KEY_RIGHT) && cursorIndex < (int)num.length()) cursorIndex++;

    // 2. Nhập ký tự (Insert vào vị trí con trỏ)
    char key = GetCharPressed();
    while (key > 0) {
        bool canAdd = false;
        if (isabc && key >= 'a' && key <= 'z' && num.length() < max_length) canAdd = true;
        if (!isabc && key >= '0' && key <= '9' && num.length() < max_length) canAdd = true;
        if (accepting_long && num.length() < max_length && key == ' ') canAdd = true; // Cho phép khoảng trắng nếu đang chấp nhận chuỗi dài

        if (canAdd) {
            std::string n_num = num;
            n_num.insert(cursorIndex, 1, (char)key);
            if (isabc) {
                int cnt = 0, mx = 0;
                for (char c : n_num) {
                    if (c >= 'a' && c <= 'z') {
                        cnt++;
                        mx = std::max(mx, cnt);
                    } else {
                        cnt = 0;
                    }
                }
                if (mx <= 30) { // Giả sử giới hạn là 30 ký tự chữ cái
                    num = n_num;
                    cursorIndex++;
                }
            } else {
                int cnt = 0, mx = 0;
                for (char c : n_num) {
                    if (c >= '0' && c <= '9') {
                        cnt++;
                        mx = std::max(mx, cnt);
                    } else {
                        cnt = 0;
                    }
                }
                if (mx <= 3) { // Giả sử giới hạn là 3 ký tự số
                    num = n_num;
                    cursorIndex++;
                }
            }
        }
        
        key = GetCharPressed();
    }

    // 3. Xóa ký tự (Backspace)
    if (IsKeyPressed(KEY_BACKSPACE) && cursorIndex > 0) {
        num.erase(cursorIndex - 1, 1);
        cursorIndex--;
    }

    // 4. Logic Cuộn (Auto-scroll)
    float padding = 20.0f;
    Vector2 sizeToCursor = MeasureTextEx(font, num.substr(0, cursorIndex).c_str(), 20, 1);
    float cursorPosX = sizeToCursor.x;

    // Nếu con trỏ vượt biên phải của box
    if (cursorPosX - scrollOffset > bounds.width - padding) {
        scrollOffset = cursorPosX - (bounds.width - padding);
    }
    // Nếu con trỏ lùi về quá biên trái
    if (cursorPosX < scrollOffset + padding) {
        scrollOffset = cursorPosX - padding;
    }
    if (scrollOffset < 0) scrollOffset = 0;

    // 5. Cursor Blink
    cursorTimer += GetFrameTime();
    if (cursorTimer >= 0.5f) {
        cursorTimer = 0;
        cursorVisible = !cursorVisible;
    }

    // Kết thúc nhập
    if (IsKeyPressed(KEY_ENTER) && !num.empty()) {
        str_value = num;
        appear = false;
        is_pending = true;
    }
}