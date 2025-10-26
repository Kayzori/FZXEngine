#pragma once

#include "../../Object2D.h"
#include "Math/Geometry/AABB2D.h"
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

    Shape2D(Shape2D* other) :
    Object2D(other->transform),
    o_verts(other->o_verts),
    o_centroid(other->o_centroid)
    {}

    Shape2D(Transform2D* transform) :
    Object2D(transform)
    {}

    Shape2D(Transform2D* transform, Shape2D* other) :
    Object2D(transform),
    o_verts(other->o_verts),
    o_centroid(other->o_centroid)
    {}

    ~Shape2D() {};

    // Methods
    std::vector<Vector2> getVertices() {                // O(n)
        return transform->apply(o_verts);
    }
    
    Vector2 getCentroid() {                             // O(1)
        return transform->apply(o_centroid);
    }
};
