#pragma once

#include <cmath>

namespace Math {

    template<typename T>
    inline T lerp(const T& a, const T& b, float t) {
        return a + t * (b - a);
    }

    inline float clamp(float value, float min, float max) {
        return std::fmax(min, std::fmin(value, max));
    }
    inline int clamp(int value, int min, int max) {
        return (value < min) ? min : (value > max) ? max : value;
    }

    inline float ceil(float value) {
        return std::ceil(value);
    }
    inline float floor(float value) {
        return std::floor(value);
    }
}


#ifdef MATH_VECTOR_UTILS

#include "Algebra/Vector2.h"
#include "Algebra/Vector3.h"
#include <algorithm>

namespace Math
{
    inline float cross(const Vector2& a, const Vector2& b) {
        return a.x * b.y - a.y * b.x;
    }
    inline float cross(const Vector3& a, const Vector3& b) {
        return a.x * b.y - a.y * b.x + a.z * b.x - a.x * b.z;
    }

    inline float dot(const Vector2& a, const Vector2& b) {
        return a.x * b.x + a.y * b.y;
    }
    inline float dot(const Vector3& a, const Vector3& b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    inline float length(const Vector2& v) {
        return std::sqrt(dot(v, v));
    }
    inline float length(const Vector3& v) {
        return std::sqrt(dot(v, v));
    }

    inline float length2(const Vector2& v) {
        return dot(v, v);
    }
    inline float length2(const Vector3& v) {
        return dot(v, v);
    }

    inline float distance(const Vector2& a, const Vector2& b) {
        return length(a - b);
    }
    inline float distance2(const Vector3& a, const Vector3& b) {
        return length2(a - b);
    }

    inline Vector2 normalize(const Vector2& v) {
        return v / length(v);
    }
    inline Vector3 normalize(const Vector3& v) {
        return v / length(v);
    }

    inline Vector2 clamp(const Vector2& t, const Vector2& a, const Vector2& b) {
        return { std::clamp(t.x, a.x, b.x), std::clamp(t.y, a.y, b.y) };
    }
    inline Vector3 clamp(const Vector3& t, const Vector3& a, const Vector3& b) {
        return { std::clamp(t.x, a.x, b.x), std::clamp(t.y, a.y, b.y), std::clamp(t.z, a.z, b.z) };
    }
};

#endif

#ifdef MATH_TRIGONOMETRY_UTILS

#include "Algebra/Vector2.h"

namespace Math
{
    const double PI = 3.14159265358979323846;
    const double PI_SQR = 9.86960440108935861883;
    const double TWO_PI = 6.28318530717958647692;

    inline float deg2rad(float degrees) {
        return degrees * PI / 180.0f;
    }
    inline float rad2deg(float radians) {
        return radians * 180.0f / PI;
    }
    
    inline Vector2 rotate(const Vector2& point, float radians) {
        float cosR = std::cos(radians);
        float sinR = std::sin(radians);
        return Vector2(
            point.x * cosR - point.y * sinR,
            point.x * sinR + point.y * cosR
        );
    }
}

#endif