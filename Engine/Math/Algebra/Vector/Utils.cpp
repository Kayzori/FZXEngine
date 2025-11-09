#include "Utils.h"

#include <cmath>
#include <algorithm>

namespace Math
{
    namespace Algebra
    {
        // Rotation
        Vector2 rotate(const Vector2& point, float radians, const Vector2& origin) {
            float s = std::sin(radians);
            float c = std::cos(radians);

            // Translate point back to origin
            float x_new = point.x - origin.x;
            float y_new = point.y - origin.y;

            // Rotate point
            float x_rotated = x_new * c - y_new * s;
            float y_rotated = x_new * s + y_new * c;

            // Translate point back
            x_rotated += origin.x;
            y_rotated += origin.y;

            return { x_rotated, y_rotated };
        }

        // Cross product
        float cross(const Vector2& a, const Vector2& b) {
            return a.x * b.y - a.y * b.x;
        }

        float cross(const Vector3& a, const Vector3& b) {
            return a.x * b.y - a.y * b.x + a.z * b.x - a.x * b.z;
        }

        // Dot product
        float dot(const Vector2& a, const Vector2& b) {
            return a.x * b.x + a.y * b.y;
        }

        float dot(const Vector3& a, const Vector3& b) {
            return a.x * b.x + a.y * b.y + a.z * b.z;
        }

        // Length
        float length(const Vector2& v) {
            return std::sqrt(dot(v, v));
        }

        float length(const Vector3& v) {
            return std::sqrt(dot(v, v));
        }

        // Squared length
        float length2(const Vector2& v) {
            return dot(v, v);
        }

        float length2(const Vector3& v) {
            return dot(v, v);
        }

        // Distance
        float distance(const Vector2& a, const Vector2& b) {
            return length(a - b);
        }

        float distance(const Vector3& a, const Vector3& b) {
            return length(a - b);
        }

        float distance2(const Vector2& a, const Vector2& b) {
            return length2(a - b);
        }

        float distance2(const Vector3& a, const Vector3& b) {
            return length2(a - b);
        }

        // Normalize
        Vector2 normalize(const Vector2& v) {
            return v / length(v);
        }

        Vector3 normalize(const Vector3& v) {
            return v / length(v);
        }

        // Clamp
        Vector2 clamp(const Vector2& t, const Vector2& a, const Vector2& b) {
            return { std::clamp(t.x, a.x, b.x), std::clamp(t.y, a.y, b.y) };
        }

        Vector3 clamp(const Vector3& t, const Vector3& a, const Vector3& b) {
            return { std::clamp(t.x, a.x, b.x), std::clamp(t.y, a.y, b.y), std::clamp(t.z, a.z, b.z) };
        }
    }
}
