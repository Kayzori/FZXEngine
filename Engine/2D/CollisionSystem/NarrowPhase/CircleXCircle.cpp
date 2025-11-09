#include "NarrowPhase.h"

#include "../Collision2D.h"

#include "Math/Algebra/Vector/Utils.h"
#include "Math/Utils.h"

#include <cmath>

namespace NarrowPhase {
    Collision2DManifold CircleXCircle(Collision2D* A, Collision2D* B) {
        Collision2DManifold manifold(A, B);

        // Cache vertices/centroids
        const auto vertsA = A->getVertices();
        const auto vertsB = B->getVertices();
        Vector2 centerA = A->getCentroid();
        Vector2 centerB = B->getCentroid();
        float radiusA = A->getRadius();
        float radiusB = B->getRadius();

        float rSum = radiusA + radiusB;
        Vector2 d = centerB - centerA;
        float dist2 = Math::Algebra::dot(d, d);

        // quick reject with squared comparison
        float rSum2 = rSum * rSum;
        if (dist2 > rSum2) return manifold; // no collision

        float dist = std::sqrt(dist2);
        if (dist < Math::EPSILON) {
            manifold.hasCollision = true;
            manifold.normal = Vector2(1.0f, 0.0f);
            manifold.penetration = rSum;
            Vector2 contact = centerA;
            manifold.addContact(contact, contact - centerA, contact - centerB, manifold.penetration);
            return manifold;
        }

        Vector2 normal = d / dist;
        manifold.hasCollision = true;
        manifold.normal = normal;
        manifold.penetration = rSum - dist;

        Vector2 contact = centerA + normal * radiusA;
        manifold.addContact(contact, contact - centerA, contact - centerB, manifold.penetration);

        return manifold;
    }
};