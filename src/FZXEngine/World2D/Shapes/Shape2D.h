#pragma once

#include "World2D/Object2D.h"
#include <vector>

// Base Class

class Shape2D : public Object2D
{
protected:
    // Properties
    std::vector<Vector2> o_verts;
    Vector2 o_centroid;
public:
    // Constructors and Destructors
    Shape2D() :
    Object2D()
    {}

    Shape2D(Transform2D* transform) :
    Object2D(transform)
    {}

    ~Shape2D() {};

    // Methods
    std::vector<Vector2> getVertices() {
        return transform->apply(o_verts);
    }
    
    Vector2 getCentroid() {
        return transform->apply(o_centroid);
    }
};
