#pragma once

#include "../Algebra/Vector/Vector2.h"

class Ray2D
{
public:
    // Properties
    Vector2 origin;
    Vector2 direction; // Should be normalized

    // Constructors and Destructors
    Ray2D(const Vector2& origin, const Vector2& normalized_direction);

    // Methods
    Vector2 pointAtParameter(float t) const;

    // Relational operators
    bool operator==(const Ray2D& other) const;
    bool operator!=(const Ray2D& other) const;
    
};