#include "NarrowPhase.h"

#include "../Collision2D.h"

#include "Math/Geometry2D/Utils.h"
#include "Math/Algebra/Vector/Utils.h"
#include "Math/Utils.h"

#include <cmath>

namespace NarrowPhase {
    Collision2DManifold CircleXPolygon(Collision2D* A, Collision2D* B) {
        // Ensure circle is circleObj, polygon is polyObj
        Collision2D* circleObj = A;
        Collision2D* polyObj = B;
        if (A->p_shape == PrimitiveShape2D::CONVEX_POLYGON2D && B->p_shape != PrimitiveShape2D::CONVEX_POLYGON2D) {
            circleObj = B;
            polyObj = A;
        }

        Collision2DManifold manifold(circleObj, polyObj);

        const auto verts = polyObj->getVertices();
        if (verts.empty()) return manifold;
        Vector2 polyCenter = polyObj->getCentroid();
        Vector2 cCenter = circleObj->getCentroid();

        // compute radius once
        const auto circleVerts = circleObj->getVertices();
        float radius = Math::Geometry::polygonRadius(circleVerts, cCenter);

        // find closest point on polygon edges (work with squared distances)
        float minDist2 = std::numeric_limits<float>::infinity();
        Vector2 closestPoint(0.0f, 0.0f);

        size_t n = verts.size();
        for (size_t i = 0; i < n; ++i) {
            const Vector2& a = verts[i];
            const Vector2& b = verts[(i + 1) % n];
            Vector2 ab = b - a;
            float ab2 = Math::Algebra::dot(ab, ab);
            float t = 0.0f;
            if (ab2 > Math::EPSILON) {
                t = Math::Algebra::dot(cCenter - a, ab) / ab2;
                if (t < 0.0f) t = 0.0f;
                else if (t > 1.0f) t = 1.0f;
            }
            Vector2 proj = a + ab * t;
            float d2 = Math::Algebra::dot(cCenter - proj, cCenter - proj);
            if (d2 < minDist2) {
                minDist2 = d2;
                closestPoint = proj;
            }
        }

        if (minDist2 > radius * radius) {
            return manifold; // not colliding
        }

        float dist = std::sqrt(minDist2);
        Vector2 normal;
        float penetration;
        if (dist < Math::EPSILON) {
            // circle center is inside polygon (or exactly on vertex)
            Vector2 dir = cCenter - polyCenter;
            if (Math::Algebra::length2(dir) < Math::EPSILON) {
                normal = Vector2(1.0f, 0.0f);
            } else {
                normal = Math::Algebra::normalize(dir);
            }
            penetration = radius; // conservative
        } else {
            normal = (cCenter - closestPoint) / dist;
            penetration = radius - dist;
        }

        manifold.hasCollision = true;
        manifold.normal = normal;
        manifold.penetration = penetration;

        Vector2 contact = closestPoint;
        manifold.addContact(contact, contact - polyCenter, contact - cCenter, penetration);

        return manifold;
    }
};