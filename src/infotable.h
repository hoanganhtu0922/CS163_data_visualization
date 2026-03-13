#include <raylib.h>
#include <string>
#include <vector>
#include "font.h"
#include <string>
#pragma once

struct infotable{
    int x, y;
    std::vector<std::string> info;
    void add(std::string new_info);
    void draw();
    std::string clicked();
};