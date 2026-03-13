#include "node.h"

bool Node::is_clicked() {
    return IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && is_hovered();
}

bool Node::is_hovered() {
    return CheckCollisionPointCircle(GetMousePosition(), { currentPos.x, currentPos.y }, (float)size / 2);
}