#pragma once

namespace Math {
    constexpr float EPSILON = 1e-6f;

    template<typename T>
    inline T lerp(const T& a, const T& b, float t) {
        return a + t * (b - a);
    }

    float clamp(float value, float min, float max);
    int clamp(int value, int min, int max);
    float ceil(float value);
    float floor(float value);

    constexpr float PI = 3.14159265358979323846f;
    constexpr float PI_SQR = 9.86960440108935861883f;
    constexpr float TWO_PI = 6.28318530717958647692f;

    float degToRad(float degrees);
    float radToDeg(float radians);
}
