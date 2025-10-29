#pragma once

#include "../../Algebra/Vector/Vector2.h"
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

    // n: number of points
                                                                    // Time Complexity
    // Constructors and Destructors
    AABB2D();                                                       // Best O(1), Average O(1), Worst O(1)
    AABB2D(const Vector2& position, const Vector2& size);           // Best O(1), Average O(1), Worst O(1)
    AABB2D(const std::vector<Vector2>& points);                     // Best O(n), Average O(n), Worst O(n)
    AABB2D(const AABB2D& other);                                    // Best O(1), Average O(1), Worst O(1)
    ~AABB2D() = default;

    //Methods
    bool contains(const Vector2& point) const;                      // Best O(1), Average O(1), Worst O(1)
    bool contains(const std::vector<Vector2>& points) const;        // Best O(n), Average O(n), Worst O(n)
    bool intersects(const std::vector<Vector2>& points) const;      // Best O(n), Average O(n), Worst O(n)
    bool intersects(const AABB2D& other) const;                     // Best O(1), Average O(1), Worst O(1)
    AABB2D merge(const AABB2D& other) const;                        // Best O(1), Average O(1), Worst O(1)

    Vector2 center() const;                                         // Best O(1), Average O(1), Worst O(1)
    Vector2 size() const;                                           // Best O(1), Average O(1), Worst O(1)
    Vector2 min() const;                                            // Best O(1), Average O(1), Worst O(1)
    Vector2 max() const;                                            // Best O(1), Average O(1), Worst O(1)
    float length() const;                                           // Best O(1), Average O(1), Worst O(1)
    float length2() const;                                          // Best O(1), Average O(1), Worst O(1)
};
