#pragma once

#include "../../Algebra/Vector/Vector2.h"

#include <vector>

class Circle2D
{
public:
    // Properties
    float radius;

    // Constructors and Destructors
    Circle2D(float radius = 1.0f);
    ~Circle2D() = default;

    // Methods
    std::vector<Vector2> getVertices(const int segments = 32, const bool top_left = false, const Vector2& origin = {0.0f, 0.0f}) const;
};
