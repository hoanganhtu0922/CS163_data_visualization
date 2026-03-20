#include "camera.h"

void camera::initialize() {
    camera.target = {0, 0};
    camera.offset = {0, 0};
    camera.zoom = 1.0f; 
    camera.rotation = 0.0f;
}

void camera::update_dragging() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        lastMouse = GetMousePosition();
    }

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        Vector2 currentMouse = GetMousePosition();
        float dx = (lastMouse.x - currentMouse.x) / camera.zoom;
        float dy = (lastMouse.y - currentMouse.y) / camera.zoom;

        camera.target.x += dx;
        camera.target.y += dy;

        lastMouse = currentMouse; 
    }
}

void camera::update_zoom() {
    float wheel = GetMouseWheelMove();

    if (wheel != 0){
        camera.zoom += wheel * 0.1f;
        if (camera.zoom < 0.1f) camera.zoom = 0.1f;
        if (camera.zoom > 5.0f) camera.zoom = 5.0f;
    }
}