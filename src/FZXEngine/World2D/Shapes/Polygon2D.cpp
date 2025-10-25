#include "Polygon2D.h"

#define MATH_VECTOR_UTILS
#include "Math/Math.h"

using namespace Math;

// Constructors

Polygon2D::Polygon2D(const std::vector<Vector2>& verts) :
    Shape2D()
{
    o_verts = verts;
    o_centroid = CalculateCentroid(verts);
}

Polygon2D::Polygon2D(Transform2D* _transform, const std::vector<Vector2>& verts) :
    Shape2D(_transform)
{
    o_verts = verts;
    o_centroid = CalculateCentroid(verts);
}

// Help Methods

Vector2 Polygon2D::CalculateCentroid(const std::vector<Vector2>& verts) const {
    // Half Sum Cross
    int n = verts.size();
    float A = 0;
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        A += cross(verts[i], verts[j]);
    }
    A *= 0.5;

    Vector2 C(0.0f, 0.0f);

    if (A == 0) return C; // 0 Division
    // Calculate Centroid
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        C.x += (verts[i].x + verts[j].x) * cross(verts[i], verts[j]);
        C.y += (verts[i].y + verts[j].y) * cross(verts[i], verts[j]);
    }
    C /= 6 * A;
    
    return C;
}
