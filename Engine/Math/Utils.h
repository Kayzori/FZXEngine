#pragma once

namespace Math {

    template<typename T>
    inline T lerp(const T& a, const T& b, float t);

    inline float clamp(float value, float min, float max);
    inline int clamp(int value, int min, int max);

    inline float ceil(float value);
    inline float floor(float value);
}

#ifdef MATH_INCLUDE_TRIGONOMETRY_UTILS

#include "Algebra/Vector/Vector2.h"

namespace Math
{
    constexpr float PI = 3.14159265358979323846f;
    constexpr float PI_SQR = 9.86960440108935861883f;
    constexpr float TWO_PI = 6.28318530717958647692f;

    inline float degToRad(float degrees);
    inline float radToDeg(float radians);
}

#endif
