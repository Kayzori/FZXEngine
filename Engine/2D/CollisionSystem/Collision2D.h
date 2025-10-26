#pragma once

#include "../Object2D.h"
#include "../World/Shapes/Shape2D.h"

class Collision2D : public Object2D
{
protected:
    // Properties
    void* parent = nullptr;
public:
    // Properties
    Shape2D* shape = nullptr;

    // Constructors and Destructors
    Collision2D(Transform2D* transform, Shape2D* shape, void* parent = nullptr);
    Collision2D(Shape2D* shape, void* parent = nullptr);
    ~Collision2D();
};

