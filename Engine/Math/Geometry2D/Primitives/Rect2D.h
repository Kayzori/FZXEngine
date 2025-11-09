#pragma once

#include "../../Algebra/Vector/Vector2.h"

#include <vector>

class Rect2D
{
public:
    // Properties
    Vector2 size;

    // Constructors and Destructors
    Rect2D(const Vector2& size) :
        size(size)
    {}

    Rect2D(const float width, const float height) :
        size(width, height)
    {}

    ~Rect2D() = default;

    // Methods
    std::vector<Vector2> getVertices(const bool top_left = false, const Vector2& origin = {0.0f, 0.0f}) const {
        if (top_left)
            return {
                {origin.x, origin.y},
                {origin.x + size.x, origin.y},
                {origin.x + size.x, origin.y + size.y},
                {origin.x, origin.y + size.y}
            };
        Vector2 hsize = size / 2;
        return {
            {origin.x - hsize.x, origin.y - hsize.y},
            {origin.x + hsize.x, origin.y - hsize.y},
            {origin.x + hsize.x, origin.y + hsize.y},
            {origin.x - hsize.x, origin.y + hsize.y}
        };
    }
};