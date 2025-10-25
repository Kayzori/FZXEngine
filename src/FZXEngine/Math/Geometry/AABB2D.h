#pragma once

#include "../Algebra/Vector2.h"
#include <vector>

// Axis-Aligned Bounding Box in 2D
class AABB2D
{
public:
    // Properties
    float x;
    float y;
    float w;
    float h;

    // Constructors and Destructors
    AABB2D();
    AABB2D(const Vector2& position, const Vector2& size);
    AABB2D(const std::vector<Vector2>& points);
    AABB2D(const AABB2D& other);
    ~AABB2D() = default;

    //Methods
    bool contains(const Vector2& point) const;
    bool contains(const std::vector<Vector2>& points) const;
    bool intersects(const std::vector<Vector2>& points) const;
    bool intersects(const AABB2D& other) const;
    AABB2D merge(const AABB2D& other) const;

    Vector2 center() const;
    Vector2 size() const;
    Vector2 min() const;
    Vector2 max() const;
    float length() const;
    float length2() const;
};
