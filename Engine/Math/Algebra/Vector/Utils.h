#pragma once

#include "Vector2.h"
#include "Vector3.h"

namespace Math
{
    namespace Algebra
    {
        // Rotation
        Vector2 rotate(const Vector2& point, float radians, const Vector2& origin = {0, 0});

        // Cross product
        float cross(const Vector2& a, const Vector2& b);
        float cross(const Vector3& a, const Vector3& b);

        // Dot product
        float dot(const Vector2& a, const Vector2& b);
        float dot(const Vector3& a, const Vector3& b);

        // Length and squared length
        float length(const Vector2& v);
        float length(const Vector3& v);

        float length2(const Vector2& v);
        float length2(const Vector3& v);

        // Distance
        float distance(const Vector2& a, const Vector2& b);
        float distance(const Vector3& a, const Vector3& b);

        float distance2(const Vector2& a, const Vector2& b);
        float distance2(const Vector3& a, const Vector3& b);

        // Normalization
        Vector2 normalize(const Vector2& v);
        Vector3 normalize(const Vector3& v);

        // Clamp
        Vector2 clamp(const Vector2& val, const Vector2& min, const Vector2& max);
        Vector3 clamp(const Vector3& val, const Vector3& min, const Vector3& max);

    }
}