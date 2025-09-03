#include "Triangulator2D.hpp"

// Internal utility functions
static float Cross(const glm::vec2& a, const glm::vec2& b) {
    return a.x * b.y - a.y * b.x;
}

// --- Private Helpers ---
bool Triangulator2D::IsCCW(const std::vector<glm::vec2>& pts) {
    double area = 0.0;
    for (size_t i = 0; i < pts.size(); ++i) {
        size_t j = (i + 1) % pts.size();
        area += pts[i].x * pts[j].y - pts[j].x * pts[i].y;
    }
    return area > 0.0;
}

std::vector<glm::vec2> Triangulator2D::RemoveCollinear(const std::vector<glm::vec2>& pts) {
    std::vector<glm::vec2> out;
    size_t n = pts.size();
    for (size_t i = 0; i < n; ++i) {
        const glm::vec2& prev = pts[(i + n - 1) % n];
        const glm::vec2& cur = pts[i];
        const glm::vec2& next = pts[(i + 1) % n];
        glm::vec2 v1 = cur - prev;
        glm::vec2 v2 = next - cur;
        if (std::fabs(Cross(v1, v2)) > 1e-9f) out.push_back(cur);
    }
    return out;
}

bool Triangulator2D::IsConvex(const glm::vec2& prev, const glm::vec2& cur, const glm::vec2& next) {
    return Cross(cur - prev, next - cur) > 0.0f; // CCW assumed
}

bool Triangulator2D::PointInTriangle(const glm::vec2& p, const glm::vec2& a, const glm::vec2& b, const glm::vec2& c) {
    float c1 = Cross(b - a, p - a);
    float c2 = Cross(c - b,p - b);
    float c3 = Cross(a - c,p - c);
    bool has_neg = (c1 < 0) || (c2 < 0) || (c3 < 0);
    bool has_pos = (c1 > 0) || (c2 > 0) || (c3 > 0);
    return !(has_neg && has_pos);
}

// --- Public Algorithms ---
std::vector<Triangle2D> Triangulator2D::EarClipping(const std::vector<glm::vec2>& polygon) {
    std::vector<Triangle2D> result;
    if (polygon.size() < 3) return result;

    std::vector<glm::vec2> poly = RemoveCollinear(polygon);
    if (poly.size() < 3) return result;
    if (!IsCCW(poly)) std::reverse(poly.begin(), poly.end());

    std::vector<int> indices(poly.size());
    for (size_t i = 0; i < poly.size(); ++i) indices[i] = (int)i;

    auto prev_idx = [&](int i) { return indices[(i - 1 + indices.size()) % indices.size()]; };
    auto next_idx = [&](int i) { return indices[(i + 1) % indices.size()]; };

    while (indices.size() > 3) {
        bool ear_found = false;
        for (size_t i = 0; i < indices.size(); ++i) {
            int pi = prev_idx((int)i);
            int ci = indices[i];
            int ni = next_idx((int)i);

            const glm::vec2& a = poly[pi];
            const glm::vec2& b = poly[ci];
            const glm::vec2& c = poly[ni];

            if (!IsConvex(a, b, c)) continue;

            bool inside = false;
            for (int idx : indices) {
                if (idx == pi || idx == ci || idx == ni) continue;
                if (PointInTriangle(poly[idx], a, b, c)) { inside = true; break; }
            }
            if (inside) continue;

            result.push_back({a, b, c});
            indices.erase(indices.begin() + i);
            ear_found = true;
            break;
        }
        if (!ear_found) break;
    }

    if (indices.size() == 3) {
        result.push_back({ poly[indices[0]], poly[indices[1]], poly[indices[2]] });
    }

    return result;
}

std::vector<Triangle2D> Triangulator2D::FanTriangulation(const std::vector<glm::vec2>& polygon) {
    std::vector<Triangle2D> result;
    if (polygon.size() < 3) return result;

    for (size_t i = 1; i + 1 < polygon.size(); ++i) {
        result.push_back({ polygon[0], polygon[i], polygon[i + 1] });
    }
    return result;
}

void Triangulator2D::DebugTriangles(const std::vector<Triangle2D>& triangles) {
    for (Triangle2D triangle : triangles) {
        std::cout << triangle;
    }
}
