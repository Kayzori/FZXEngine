#pragma once

#include <cmath>
#include <array>
#include <algorithm>
#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define PI      3.14159265358979323846f
#define EulerN  2.71828182845904523536f

inline float deg2rad(float degrees) {return degrees * PI / 180.0f;}
inline float rad2deg(float radians) {return radians * 180 / PI;}
