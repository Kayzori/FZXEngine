#pragma once
#include "../Object.hpp"

class Object2D : public Object {
public:
    // Con/De structor
    Object2D() {
        transform = new Transform2D();
    }

    ~Object2D() {
        transform = new Transform2D();
        delete transform;
    }

    // Properties
    Transform2D* transform;
};
