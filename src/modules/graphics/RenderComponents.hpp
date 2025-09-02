#pragma once

#include <string>

#include "Texture.h"

// Built in component
struct Position {
    float x, y;
};

// Built in component
struct Scale {
    float x, y;
};

// Built in component
struct Quad {
    float width, height;
};

// Built in component
struct Sprite {
    nebula::graphics::Texture* texture;
};

// Built in component
// TODO Color component
// struct Color { 
// };