#pragma once

#include "Math/Geometry/Transform2D.h"

// Base Class

class Object2D
{
private:
    bool transformOwned;
public:
    // Properties
    Transform2D* transform;

    // Constructors and Destructors
    Object2D() :
    transform(new Transform2D()),
    transformOwned(true)
    {}

    Object2D(Transform2D* transform) :
    transform(new Transform2D()),
    transformOwned(true)
    {}
    
    virtual ~Object2D() {
        if (transformOwned) delete transform;
    }

    // Methods
    virtual void update(double dt) = 0;
    virtual void render() = 0;
};
