#include "raylib.h"
#include <string>

struct ScrollableTextbox {
    Rectangle bounds;       // Vị trí và kích thước khung
    std::string text;       // Nội dung chuỗi
    int cursorIndex = 0;    // Vị trí con trỏ (tính theo số ký tự)
    float scrollOffset = 0; // Độ dịch chuyển của chữ (tính theo pixel)
    bool active = false;    // Trạng thái focus
    int fontSize = 25;

    void Update() {
        // 1. Kiểm tra Click chuột để kích hoạt Textbox
        if (CheckCollisionPointRec(GetMousePosition(), bounds)) {
            SetMouseCursor(MOUSE_CURSOR_IBEAM);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) active = true;
        } else {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) active = false;
        }

        if (active) {
            // 2. Nhập ký tự (Insert)
            int key = GetCharPressed();
            while (key > 0) {
                if ((key >= 32) && (key <= 125)) {
                    text.insert(cursorIndex, 1, (char)key);
                    cursorIndex++;
                }
                key = GetCharPressed();
            }

            // 3. Di chuyển con trỏ (Mũi tên trái/phải)
            if (IsKeyPressed(KEY_LEFT) && cursorIndex > 0) cursorIndex--;
            if (IsKeyPressed(KEY_RIGHT) && cursorIndex < (int)text.length()) cursorIndex++;
            
            // Phím Home/End
            if (IsKeyPressed(KEY_HOME)) cursorIndex = 0;
            if (IsKeyPressed(KEY_END)) cursorIndex = (int)text.length();

            // 4. Xóa ký tự (Backspace/Delete)
            if (IsKeyPressed(KEY_BACKSPACE) && cursorIndex > 0) {
                text.erase(cursorIndex - 1, 1);
                cursorIndex--;
            }
            if (IsKeyPressed(KEY_DELETE) && cursorIndex < (int)text.length()) {
                text.erase(cursorIndex, 1);
            }

            // 5. Logic Tự động cuộn (Auto-scroll)
            float padding = 10.0f; // Khoảng cách đệm ở 2 đầu
            float cursorPixelPos = (float)MeasureText(text.substr(0, cursorIndex).c_str(), fontSize);

            // Nếu con trỏ đi quá biên phải của box -> tăng scrollOffset
            if (cursorPixelPos - scrollOffset > bounds.width - padding) {
                scrollOffset = cursorPixelPos - (bounds.width - padding);
            }
            // Nếu con trỏ lùi về quá biên trái -> giảm scrollOffset
            if (cursorPixelPos < scrollOffset + padding) {
                scrollOffset = cursorPixelPos - padding;
            }

            if (scrollOffset < 0) scrollOffset = 0;
        }
    }

    void Draw() {
        // Vẽ nền và viền
        DrawRectangleRec(bounds, active ? WHITE : RAYWHITE);
        DrawRectangleLinesEx(bounds, 2, active ? BLUE : DARKGRAY);

        // --- Bắt đầu chế độ Scissor (Cắt chữ thừa) ---
        BeginScissorMode((int)bounds.x, (int)bounds.y, (int)bounds.width, (int)bounds.height);
            
            float startX = bounds.x + 5 - scrollOffset;
            float posY = bounds.y + (bounds.height / 2) - (fontSize / 2);

            // Vẽ văn bản
            DrawText(text.c_str(), (int)startX, (int)posY, fontSize, BLACK);

            // Vẽ con trỏ nhấp nháy "|"
            if (active && (((int)(GetTime() * 3) % 2) == 0)) {
                float cursorX = startX + MeasureText(text.substr(0, cursorIndex).c_str(), fontSize);
                // Dùng DrawLine để làm con trỏ thanh mảnh và chuyên nghiệp hơn
                DrawLineEx({cursorX, posY}, {cursorX, posY + fontSize}, 2, RED);
            }

        EndScissorMode();
        // --- Kết thúc chế độ Scissor ---
    }
};