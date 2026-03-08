#include "raylib.h"
#include <vector>
#include <string>
#include <vector>
#include <iostream>
#include "raymath.h"

struct Slot {
    int value;
    bool occupied = false;
    Vector2 currentPos;
    Vector2 targetPos;
    float alpha = 0.0f;
    int carry_value;
    bool highlighting = false; 
};

class LinearProbingVisualizer {
private:
    static const int TABLE_SIZE = 10;
    Slot table[TABLE_SIZE];
    float smoothSpeed = 0.1f;

public:
    int is_inserting = false, carrying_value, carrying_index;
    LinearProbingVisualizer();
    int hashFunction(int key);
    void insert();
    void update();
    void draw();
    void run();
};