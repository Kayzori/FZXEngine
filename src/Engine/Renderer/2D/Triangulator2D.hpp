#pragma once

#include <Math/Math.hpp>

struct Triangle2D {
    glm::vec2 a;
    glm::vec2 b;
    glm::vec2 c;
    bool operator==(const Triangle2D& other) const {
        std::array<glm::vec2, 3> v1 = {a, b, c};
        std::array<glm::vec2, 3> v2 = {other.a, other.b, other.c};
        std::sort(v1.begin(), v1.end(), [](const glm::vec2& l, const glm::vec2& r) {
            return std::tie(l.x, l.y) < std::tie(r.x, r.y);
        });
        std::sort(v2.begin(), v2.end(), [](const glm::vec2& l, const glm::vec2& r) {
            return std::tie(l.x, l.y) < std::tie(r.x, r.y);
        });
        return v1 == v2;
    }
};

inline std::ostream& operator<<(std::ostream& os, const Triangle2D& tri) {
    os << "Triangle: ("
       << tri.a.x << ", " << tri.a.y << ") ("
       << tri.b.x << ", " << tri.b.y << ") ("
       << tri.c.x << ", " << tri.c.y << ")";
    return os;
}

class Triangulator2D {
public:
    // Convex Polygon Triangulation Algorithm (    O(n) Time    )
    static std::vector<Triangle2D> FanTriangulation(const std::vector<glm::vec2>& Polygon);
    // Convex/Concave Polygon Triangulation Algorithm (    O(n²) Time    )
    static std::vector<Triangle2D> EarClipping(const std::vector<glm::vec2>& ConvexPolygon);
    // Convex/Concave/Hole Polygon Triangulation Algorithm (    O(nlog(n)) Time    )
    // TODO
    static std::vector<Triangle2D> CDT(const std::vector<glm::vec2>& polygon);
    // Debug Triangles Points
    static void DebugTriangles(const std::vector<Triangle2D>& triangles);
private:
    static bool IsCCW(const std::vector<glm::vec2>& pts);
    static std::vector<glm::vec2> RemoveCollinear(const std::vector<glm::vec2>& pts);
    static bool IsConvex(const glm::vec2& prev, const glm::vec2& cur, const glm::vec2& next);
    static bool PointInTriangle(const glm::vec2& p, const glm::vec2& a, const glm::vec2& b, const glm::vec2& c);
};

