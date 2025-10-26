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
    extern const double PI;
    extern const double PI_SQR;
    extern const double TWO_PI;

    inline float deg2rad(float degrees);
    inline float rad2deg(float radians);
}

#endif
