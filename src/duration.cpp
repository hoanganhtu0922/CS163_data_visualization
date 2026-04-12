#include "duration.h"

void duration::initialize(int _x, int _y, int _length, int _value) { 
    Color darkerRed = { 200, 0, 0, 255 }; // Darker red color
    x = _x;
    y = _y; 
    length = _length;
    value = _value;
    progressBarColor = RED; // YouTube red
    hoverProgressBarColor = darkerRed; // Darker red
    backgroundColor = LIGHTGRAY;
    hoverBackgroundColor = DARKGRAY;
    handleColor = RED;
    hoverHandleColor = RED;
    barHeight = 10.0f; // Thin bar
    handleRadius = 15.0f; // Large handle
    isHoveringBar = false;
    isHoveringHandle = false;
    isDraggingHandle = false;
}

void duration::draw() {
    float handleX = x + value;
    float handleY = y + barHeight / 2.0f;

    DrawRectangle((float)x, (float)y, (float)length, barHeight, isHoveringBar ? hoverBackgroundColor : backgroundColor);
    DrawRectangle((float)x, (float)y, (float)value, barHeight, isHoveringBar ? hoverProgressBarColor : progressBarColor);
    DrawCircle(handleX, handleY, isDraggingHandle ? handleRadius + 2.0f : handleRadius, isHoveringHandle ? hoverHandleColor : handleColor);
    if (length == 200) {
        assets::Instance().draw_texture(!is_paused ? "pause_icon" : "play_icon", { (float)x - 80, (float)(y - 26) }, 0.15f);
    }
}

void duration::update() {
    Vector2 mousePos = GetMousePosition();

    float handleX = x + value;
    float handleY = y + barHeight / 2.0f;

    isHoveringBar = CheckCollisionPointRec(mousePos, { (float)x, (float)y - handleRadius, (float)length, barHeight + handleRadius * 2.0f });

    isHoveringHandle = CheckCollisionPointCircle(mousePos, { handleX, handleY }, handleRadius + 20);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (isHoveringHandle) {
            isDraggingHandle = true;
            is_changed = true;
        } else if (isHoveringBar) {
            value = mousePos.x - x;
            is_changed = true;
        }
    } else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        isDraggingHandle = false;
    }

    if (isDraggingHandle) {
        value = mousePos.x - x;
        is_changed = true;
    }

    if (assets::Instance().is_clicked({ (float)x - 80, (float)(y - 26) }, "play_icon", 0.15f)) {
        is_paused = !is_paused;
        is_changed = true;
    }

    if (value < 0) value = 0;
    if (value > length) value = length;
    if (is_paused) is_changed = true;
}