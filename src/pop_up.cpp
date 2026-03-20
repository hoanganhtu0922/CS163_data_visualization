#include "pop_up.h"

void pop_up::draw() {
    if (showPopup) {
        // Draw Pop-up Box
        Rectangle popupBox = { (float)x, (float)y, 300, 150 };
        DrawRectangleRec(popupBox, RAYWHITE);
        DrawRectangleLinesEx(popupBox, 2, GRAY);

        DrawTextEx(font, "ENTER YOUR NUMBER:", {(float)(x + 20), (float)(y + 20)}, 20, 1, DARKGRAY);
        
        // Draw Input Field
        DrawRectangle((float)(x + 20), (float)(y + 60), 260, 40, LIGHTGRAY);
        DrawTextEx(font, inputBuffer, {(float)(x + 30), (float)(y + 70)}, 20, 1, RED);
        
        DrawTextEx(font, "Press ENTER to save, Press C to cancel", {(float)(x + 20), (float)(y + 110)}, 15, 1, DARKGRAY);
    }
}   

void pop_up::update() {
    if (showPopup) {
        // Get char pressed (unicode)
        int key = GetCharPressed();

        // Check if the character is a number (0-9)
        if ((key >= 48) && (key <= 57) && (letterCount < 6)) {
            inputBuffer[letterCount] = (char)key;
            inputBuffer[letterCount + 1] = '\0'; // Add null terminator
            letterCount++;
        }

        // Handle Backspace
        if (IsKeyPressed(KEY_BACKSPACE)) {
            letterCount--;
            if (letterCount < 0) letterCount = 0;
            inputBuffer[letterCount] = '\0';
        }

        // Close and "Submit"
        if (IsKeyPressed(KEY_C) || (IsKeyPressed(KEY_ENTER) && letterCount > 0)) {
            if (IsKeyPressed(KEY_ENTER) && letterCount > 0) {
                value = 0;
                for (int i = 0; i < letterCount; i++) {
                    value = value * 10 + (inputBuffer[i] - '0');
                }
                is_pending = true; 
            } else {
                is_pending = false;
            }
            showPopup = false;
        }
    }
}