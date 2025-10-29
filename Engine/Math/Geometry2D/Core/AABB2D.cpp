#include "AABB2D.h"

#include <algorithm>
#include <limits>
#include <cmath>

// Constructors
AABB2D::AABB2D() : x(0), y(0), w(0), h(0) {}

AABB2D::AABB2D(const Vector2& position, const Vector2& size)
    : x(static_cast<int>(position.x)), y(static_cast<int>(position.y)),
      w(static_cast<int>(size.x)), h(static_cast<int>(size.y)) {}


AABB2D::AABB2D(const std::vector<Vector2>& points) {
    if (points.empty()) {
        x = y = w = h = 0;
        return;
    }

    float minX = std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float maxY = std::numeric_limits<float>::lowest();

    for (const auto& point : points) {
        if (point.x < minX) minX = point.x;
        if (point.y < minY) minY = point.y;
        if (point.x > maxX) maxX = point.x;
        if (point.y > maxY) maxY = point.y;
    }

    x = minX;
    y = minY;
    w = maxX - minX;
    h = maxY - minY;
}

AABB2D::AABB2D(const AABB2D& other)
    : x(other.x), y(other.y), w(other.w), h(other.h) {}


// Methods

bool AABB2D::contains(const Vector2& point) const {
    return point.x >= x && point.x <= x + w &&
           point.y >= y && point.y <= y + h;
}

bool AABB2D::contains(const std::vector<Vector2>& points) const {
    for (const auto& point : points) {
        if (!contains(point)) {
            return false;
        }
    }
    return true;
}

bool AABB2D::intersects(const std::vector<Vector2>& points) const {
    for (const auto& point : points) {
        if (contains(point)) {
            return true;
        }
    }
    return false;
}

bool AABB2D::intersects(const AABB2D& other) const {
    return !(x > other.x + other.w || x + w < other.x ||
             y > other.y + other.h || y + h < other.y);
}

AABB2D AABB2D::merge(const AABB2D& other) const {
    int minX = std::min(x, other.x);
    int minY = std::min(y, other.y);
    int maxX = std::max(x + w, other.x + other.w);
    int maxY = std::max(y + h, other.y + other.h);

    return AABB2D(Vector2(minX, minY),
                  Vector2(maxX - minX, maxY - minY));
}

Vector2 AABB2D::center() const {
    return Vector2(x + w / 2.0f, y + h / 2.0f);
}

Vector2 AABB2D::size() const {
    return Vector2(w, h);
}

Vector2 AABB2D::min() const {
    return Vector2(x, y);
}

Vector2 AABB2D::max() const {
    return Vector2(x + w, y + h);
}

float AABB2D::length() const {
    return std::sqrt(w * w + h * h);
}

float AABB2D::length2() const {
    return w * w + h * h;
}
