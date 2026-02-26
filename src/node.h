#include "raylib.h"
struct Node {
    int data;
    Node* next;
    Vector2 currentPos; 
    Vector2 targetPos;  
    float alpha;       
};