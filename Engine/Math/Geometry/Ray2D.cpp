#include "Ray2D.h"

// Constructors and Destructors
Ray2D::Ray2D(const Vector2& origin, const Vector2& normalized_direction)
    : origin(origin), direction(normalized_direction) {}

// Methods
Vector2 Ray2D::pointAtParameter(float t) const {
    return Vector2(origin.x + direction.x * t, origin.y + direction.y * t);
}

// Relational operators
bool Ray2D::operator==(const Ray2D& other) const {
    return (origin == other.origin) && (direction == other.direction);
}
bool Ray2D::operator!=(const Ray2D& other) const {
    return !(*this == other);
}