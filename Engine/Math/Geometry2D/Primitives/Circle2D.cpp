#include "Circle2D.h"

#define MATH_INCLUDE_TRIGONOMETRY_UTILS
#include "../../Utils.h"

Circle2D::Circle2D(float radius):
    radius(radius)
{}

std::vector<Vector2> Circle2D::getVertices(const int segments, const bool top_left, const Vector2& origin) const
{
    std::vector<Vector2> vertices;
    vertices.reserve(segments);

    float angleIncrement = static_cast<float>(Math::TWO_PI) / static_cast<float>(segments);

    for (int i = 0; i < segments; ++i)
    {
        float angle = i * angleIncrement;
        float x = radius * cosf(angle) + origin.x;
        float y = radius * sinf(angle) + origin.y;
        if (top_left) {
            x += radius;
            y += radius;
        }
        vertices.emplace_back(x, y);
    }

    return vertices;
}

