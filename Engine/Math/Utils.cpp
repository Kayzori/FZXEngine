#include "Utils.h"

#include <limits>
#include <cmath>
#include <algorithm>

namespace Math {

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

    float degToRad(float degrees) {
        return degrees * static_cast<float>(PI / 180.0f);
    }

    float radToDeg(float radians) {
        return radians * static_cast<float>(180.0f / PI);
    }
} // namespace Math
