#include "Object2D.hpp"

// Con/De structor
Object2D::Object2D() {
    transform = new Transform2D();
}

Object2D::~Object2D() {
    if (transform) delete transform;
}