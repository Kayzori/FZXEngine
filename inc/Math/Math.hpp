#pragma once

#include <cmath>
#include <array>
#include <limits>
#include <algorithm>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <string>
#include <chrono>
#include <random>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/geometric.hpp>

#define PI      3.14159265358979323846f
#define EulerN  2.71828182845904523536f
#define EPS     1e-6f

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

inline std::ostream& operator<<(std::ostream& os, const std::vector<glm::vec2>& vecs) {
    for (glm::vec2 vec : vecs) {
        os << '(' << vec.x << ", " << vec.y << ')';
        os << '\n';
    }
    return os;
}

struct AABB {
    float x{0.0f};
    float y{0.0f};
    float hw{0.0f};
    float hh{0.0f};
    glm::vec2 center;
    glm::vec2 halfSize;
    glm::vec2 max;
    glm::vec2 min;

    AABB() = default;

    AABB(float _x, float _y, float _w, float _h) :
    x(_x),
    y(_y),
    hw(_w),
    hh(_h),
    center({_x, _y}),
    halfSize({_w, _h})
    {
        max = {x + hw, y + hh};
        min = {x - hw, y - hh};
    }

    AABB(float _x, float _y, float _halfSize):
    x(_x),
    y(_y),
    hw(_halfSize),
    hh(_halfSize),
    center({_x, _y}),
    halfSize({_halfSize, _halfSize})
    {}

    AABB(glm::vec2 _center, glm::vec2 _halfSize):
    x(_center.x),
    y(_center.y),
    hw(_halfSize.x),
    hh(_halfSize.y),
    center(_center),
    halfSize(_halfSize)
    {}

    AABB(glm::vec2 _center, float _halfSize):
    x(_center.x),
    y(_center.y),
    hw(_halfSize),
    hh(_halfSize),
    center(_center),
    halfSize({_halfSize, _halfSize})
    {}

    AABB(const std::vector<glm::vec2>& verts) {
        if (verts.empty()) {
            x = y = hw = hh = 0.0f;
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
        hw = (maxX - minX) * 0.5f;
        hh = (maxY - minY) * 0.5f;
        center = {x, y};
        halfSize = {hw, hh};
    }

    std::vector<glm::vec2> getVertices() {
        return {
            {x - hw, y - hh}, // top-left
            {x + hw, y - hh}, // top-right
            {x + hw, y + hh}, // bottom-right
            {x - hw, y + hh}  // bottom-left
        };
    }

    bool contains(const glm::vec2 point) const {
        return point.x >= x - hw && point.x <= x + hw &&
               point.y >= y - hh && point.y <= y + hh;
    }

    bool contains(const std::vector<glm::vec2> points) const {
        for (glm::vec2 point : points) {
            if (!contains(point)) return false;
        }
        return true;
    }

    bool contains(const AABB& other) const {
        return (other.x - other.hw >= x - hw) &&
            (other.x + other.hw <= x + hw) &&
            (other.y - other.hh >= y - hh) &&
            (other.y + other.hh <= y + hh);
    }

    bool atLeastContains(const std::vector<glm::vec2> points) const {
        for (glm::vec2 point : points) {
            if (contains(point)) return true;
        }
        return false;
    }

    bool atLeastContains(const glm::vec2 point) const {
        if (contains(point)) return true;
        return false;
    }

    bool intersects(const AABB& other) const {
        return !(other.x - other.hw > x + hw ||
                 other.x + other.hw < x - hw ||
                 other.y - other.hh > y + hh ||
                 other.y + other.hh < y - hh);
    }

    bool operator==(const AABB& other) const {
        return x == other.x && 
               y == other.y && 
               hw == other.hw && 
               hh == other.hh;
    }
    
    bool operator!=(const AABB& other) const {
        return x != other.x && 
               y != other.y && 
               hw != other.hw && 
               hh != other.hh;
    }
};

struct Edge2D {
    glm::vec2 p1, p2;

    Edge2D(glm::vec2 _p1, glm::vec2 _p2) : p1(_p1), p2(_p2) {}

    glm::vec2 getClosestPoint(const glm::vec2& point) const {
        glm::vec2 ab = p2 - p1;
        float abLengthSq = glm::dot(ab, ab);
        
        if (abLengthSq < 1e-8f) return p1;
        
        float t = glm::dot(point - p1, ab) / abLengthSq;
        t = std::clamp(t, 0.0f, 1.0f);
        return p1 + t * ab;
    }

    bool hasPoint(const glm::vec2& point, float tolerance = 1e-6f) const {
        float minX = std::min(p1.x, p2.x);
        float maxX = std::max(p1.x, p2.x);
        float minY = std::min(p1.y, p2.y);
        float maxY = std::max(p1.y, p2.y);
        
        if (point.x < minX - tolerance || point.x > maxX + tolerance ||
            point.y < minY - tolerance || point.y > maxY + tolerance) {
            return false;
        }
        
        glm::vec2 ap = point - p1;
        glm::vec2 ab = p2 - p1;
        
        float cross = std::abs(ap.x * ab.y - ap.y * ab.x);
        if (cross > tolerance * glm::length(ab)) {
            return false;
        }
        
        float dot = glm::dot(ap, ab);
        return dot >= -tolerance && dot <= glm::dot(ab, ab) + tolerance;
    }

    glm::vec2 getNormal() const {
        glm::vec2 ab = p2 - p1;
        return glm::normalize(glm::vec2(-ab.y, ab.x));
    }

    float length() const {
        return glm::distance(p1, p2);
    }

    float lengthSquared() const {
        return glm::dot(p2 - p1, p2 - p1);
    }

    glm::vec2 getPoint(float t) const {
        t = std::clamp(t, 0.0f, 1.0f);
        return p1 + t * (p2 - p1);
    }

    void swap() {
        glm::vec2 t = p1;
        p1 = p2;
        p2 = t;
    }
};

inline float deg2rad(float degrees) {return degrees * PI / 180.0f;}
inline float rad2deg(float radians) {return radians * 180 / PI;}

inline glm::vec2 rotate_local_to_world(const glm::vec2 v_local, float rotation_degrees) {
    float r = glm::radians(rotation_degrees);
    float c = cosf(r), s = sinf(r);
    return glm::vec2(c * v_local.x - s * v_local.y, s * v_local.x + c * v_local.y);
}

inline glm::vec2 perp(const glm::vec2 v) {
    return glm::vec2(-v.y, v.x);
}

inline float cross(const glm::vec2 &a, const glm::vec2 &b) {
    return a.x * b.y - a.y * b.x;
}
