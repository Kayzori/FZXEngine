#include "Utils.h"

#include "../Algebra/Vector/Utils.h"

namespace Math
{
    namespace Geometry
    {
        Vector2 polygonCentroid(const std::vector<Vector2>& verts) {
            // Half Sum Cross
            int n = verts.size();
            float A = 0;
            for (int i = 0; i < n; i++) {
                int j = (i + 1) % n;
                A += Math::Algebra::cross(verts[i], verts[j]);
            }
            A *= 0.5;

            Vector2 C(0.0f, 0.0f);

            if (A == 0) return C; // 0 Division
            // Calculate Centroid
            for (int i = 0; i < n; i++) {
                int j = (i + 1) % n;
                C.x += (verts[i].x + verts[j].x) * Math::Algebra::cross(verts[i], verts[j]);
                C.y += (verts[i].y + verts[j].y) * Math::Algebra::cross(verts[i], verts[j]);
            }
            C /= 6 * A;
            
            return C;
        }
    }
}