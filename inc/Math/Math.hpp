#pragma once

#include <cmath>
#include <array>
#include <limits>
#include <algorithm>
#include <iostream>
#include <vector>
#include <unordered_set>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/geometric.hpp>

#define PI      3.14159265358979323846f
#define EulerN  2.71828182845904523536f

inline std::ostream& operator<<(std::ostream& os, const glm::vec2& vec) {
    os << '(' << vec.x << ", " << vec.y << ')';
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const glm::vec3& vec) {
    os << '(' << vec.x << ", " << vec.y << ", " << vec.z << ')';
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const glm::vec4& vec) {
    os << '(' << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ')';
    return os;
}

struct AABB {
    float x{0.0f};
    float y{0.0f};
    float w{0.0f};
    float h{0.0f};
    glm::vec2 center;
    glm::vec2 halfSize;

    // Default constructor
    AABB() = default;

    // Constructor with position and size
    AABB(float _x, float _y, float _w, float _h) :
    x(_x),
    y(_y),
    w(_w),
    h(_h),
    center({_x, _y}),
    halfSize({_w, _h})
    {}

    AABB(float _x, float _y, float _halfSize):
    x(_x),
    y(_y),
    w(_halfSize),
    h(_halfSize),
    center({_x, _y}),
    halfSize({_halfSize, _halfSize})
    {}

    AABB(glm::vec2 _center, glm::vec2 _halfSize):
    x(_center.x),
    y(_center.y),
    w(_halfSize.x),
    h(_halfSize.y),
    center(_center),
    halfSize(_halfSize)
    {}

    AABB(glm::vec2 _center, float _halfSize):
    x(_center.x),
    y(_center.y),
    w(_halfSize),
    h(_halfSize),
    center(_center),
    halfSize({_halfSize, _halfSize})
    {}

    // Constructor from vertices
    AABB(const std::vector<glm::vec2>& verts) {
        if (verts.empty()) {
            x = y = w = h = 0.0f;
            return;
        }

        float minX = verts[0].x;
        float minY = verts[0].y;
        float maxX = verts[0].x;
        float maxY = verts[0].y;

        for (const auto& v : verts) {
            if (v.x < minX) minX = v.x;
            if (v.y < minY) minY = v.y;
            if (v.x > maxX) maxX = v.x;
            if (v.y > maxY) maxY = v.y;
        }

        x = (minX + maxX) * 0.5f;
        y = (minY + maxY) * 0.5f;
        w = (maxX - minX) * 0.5f;
        h = (maxY - minY) * 0.5f;
        center = {x, y};
        halfSize = {w, h};
    }

    std::vector<glm::vec2> getVertices() {
        return {
            {x - w, y - h}, // top-left
            {x + w, y - h}, // top-right
            {x + w, y + h}, // bottom-right
            {x - w, y + h}  // bottom-left
        };
    }

    bool contains(const glm::vec2 point) const {
        return point.x >= x - w && point.x <= x + w &&
               point.y >= y - h && point.y <= y + h;
    }

    bool contains(const std::vector<glm::vec2> points) const {
        for (glm::vec2 point : points) {
            if (contains(point)) return true;
        }
        return false;
    }

    bool intersects(const AABB& other) const {
        return !(other.x - other.w > x + w ||
                 other.x + other.w < x - w ||
                 other.y - other.h > y + h ||
                 other.y + other.h < y - h);
    }
};

inline float deg2rad(float degrees) {return degrees * PI / 180.0f;}
inline float rad2deg(float radians) {return radians * 180 / PI;}
