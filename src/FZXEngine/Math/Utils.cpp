#include "Utils.h"

#include <cmath>
#include <algorithm>

namespace Math {

    template<typename T>
    T lerp(const T& a, const T& b, float t) {
        return a + t * (b - a);
    }

    float clamp(float value, float min, float max) {
        return std::fmax(min, std::fmin(value, max));
    }

    int clamp(int value, int min, int max) {
        return (value < min) ? min : (value > max) ? max : value;
    }

    float ceil(float value) {
        return std::ceil(value);
    }

    float floor(float value) {
        return std::floor(value);
    }
}

#ifdef MATH_TRIGONOMETRY_UTILS

namespace Math {

    const double PI = 3.14159265358979323846;
    const double PI_SQR = 9.86960440108935861883;
    const double TWO_PI = 6.28318530717958647692;

    float deg2rad(float degrees) {
        return degrees * static_cast<float>(PI / 180.0);
    }

    float rad2deg(float radians) {
        return radians * static_cast<float>(180.0 / PI);
    }
}

#endif