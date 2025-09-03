#pragma once
#include "../Object.hpp"
#include <Engine/Component/Transform2D.hpp>

class Object2D : public Object {
public:
    // Con/De structor
    Object2D();
    ~Object2D();

    // Properties
    Transform2D* transform;
};
