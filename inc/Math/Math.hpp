#pragma once

#include <cmath>

#define PI      3.14159265358979323846f
#define EulerN  2.71828182845904523536f

float deg2rad(float degrees) {
    return degrees * PI / 180.0f;
}

float rad2deg(float radians) {
    return radians * 180 / PI;
}
