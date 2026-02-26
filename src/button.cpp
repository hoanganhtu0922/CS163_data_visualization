#include "button.h"
button::button(int x, int y, int width, int height, Color color, std::string text) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->color = color;
    this->text = text;
}

void button::draw() {
    DrawRectangleRounded({ (float)x, (float)y, (float)width, (float)height }, 0.2f, 4, color);
    DrawRectangleRoundedLines({ (float)x, (float)y, (float)width, (float)height }, 0.2f, 4, DARKGRAY);
    Vector2 textSize = MeasureTextEx(font, text.c_str(), 20, 1);
    DrawTextEx(font, text.c_str(), { (float)(x + (width - textSize.x) / 2), (float)(y + (height - textSize.y) / 2) }, 20, 1, WHITE);
}

bool button::isClicked() {
    Vector2 mousePos = GetMousePosition();
    return CheckCollisionPointRec(mousePos, { (float)x, (float)y, (float)width, (float)height }) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

bool button::isHovered() {
    Vector2 mousePos = GetMousePosition();
    return CheckCollisionPointRec(mousePos, { (float)x, (float)y, (float)width, (float)height });
}