#include "Utils.h"

#include "../Algebra/Vector/Utils.h"
#include "../Utils.h"
#include <cmath>

namespace Math
{
    namespace Geometry
    {
        float polygonArea(const std::vector<Vector2>& verts) {
            size_t n = verts.size();
            float A = 0;
            for (int i = 0; i < n; i++) {
                int j = (i + 1) % n;
                A += Math::Algebra::cross(verts[i], verts[j]);
            }
            return A;
        }

        Vector2 polygonCentroid(const std::vector<Vector2>& verts) {
            // Half Sum Cross
            size_t n = verts.size();
            float A = polygonArea(verts) * 0.5f;

            Vector2 C(0.0f, 0.0f);

            if (std::fabs(A) < Math::EPSILON) return C; // avoid division by near-zero area
            // Calculate Centroid
            for (int i = 0; i < n; i++) {
                int j = (i + 1) % n;
                C.x += (verts[i].x + verts[j].x) * Math::Algebra::cross(verts[i], verts[j]);
                C.y += (verts[i].y + verts[j].y) * Math::Algebra::cross(verts[i], verts[j]);
            }
            C /= 6 * A;
            
            return C;
        }

        float polygonRadius(const std::vector<Vector2>& verts, const Vector2& from) {
            float max_dist = 0.0f;
            // Brute force all distance checks
            for (const Vector2& vert : verts) {
                float dist = Math::Algebra::distance(from, vert);
                if (max_dist < dist) max_dist = dist;
            }
            return max_dist;
        }
    }
}