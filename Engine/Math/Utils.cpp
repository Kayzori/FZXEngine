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

#ifdef MATH_INCLUDE_TRIGONOMETRY_UTILS

namespace Math {
    float degToRad(float degrees) {
        return degrees * static_cast<float>(PI / 180.0);
    }

    float radToDeg(float radians) {
        return radians * static_cast<float>(180.0 / PI);
    }
}

#endif