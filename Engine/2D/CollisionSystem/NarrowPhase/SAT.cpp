#include "NarrowPhase.h"

#include "../Collision2D.h"

#include "Math/Algebra/Vector/Utils.h"
#include "Math/Utils.h"

#include <cmath>

namespace NarrowPhase {
    Collision2DManifold SAT(Collision2D* A, Collision2D* B) {
        Collision2DManifold manifold(A, B);

        // Cache frequently used data
        const auto vertsA = A->getVertices();
        const auto vertsB = B->getVertices();

        float smallestPenetration = std::numeric_limits<float>::infinity();
        Vector2 smallestAxisNormalized(0.0f, 0.0f);

        Vector2 centerA = A->getCentroid();
        Vector2 centerB = B->getCentroid();
        Vector2 centerDir = centerB - centerA;

        // Inline lambda to test a single (unnormalized) axis.
        auto testAxis = [&](const Vector2& axisUnnorm) -> bool {
            // degenerate axis skip
            float axisLen2 = Math::Algebra::dot(axisUnnorm, axisUnnorm);
            if (axisLen2 < Math::EPSILON) return true;

            // project vertsA onto axisUnnorm
            float minA = std::numeric_limits<float>::infinity();
            float maxA = -std::numeric_limits<float>::infinity();
            for (const auto& v : vertsA) {
                float p = Math::Algebra::dot(v, axisUnnorm);
                if (p < minA) minA = p;
                if (p > maxA) maxA = p;
            }

            // project vertsB onto axisUnnorm
            float minB = std::numeric_limits<float>::infinity();
            float maxB = -std::numeric_limits<float>::infinity();
            for (const auto& v : vertsB) {
                float p = Math::Algebra::dot(v, axisUnnorm);
                if (p < minB) minB = p;
                if (p > maxB) maxB = p;
            }

            // overlap in unnormalized space
            float overlapUnnorm = std::min(maxA, maxB) - std::max(minA, minB);
            if (overlapUnnorm <= 0.0f) {
                // Separating axis -> no collision
                return false;
            }

            // convert overlap into world-space distance by dividing by |axis|
            float invLen = 1.0f / std::sqrt(axisLen2);
            float penetration = overlapUnnorm * invLen;

            if (penetration < smallestPenetration) {
                smallestPenetration = penetration;
                Vector2 axisNorm = axisUnnorm * invLen;
                float dirTest = Math::Algebra::dot(centerDir, axisNorm);
                if (dirTest < 0.0f) {
                    smallestAxisNormalized = Vector2(-axisNorm.x, -axisNorm.y);
                } else {
                    smallestAxisNormalized = axisNorm;
                }
            }

            return true;
        };

        // test A edge normals
        const size_t nA = vertsA.size();
        for (size_t i = 0; i < nA; ++i) {
            const Vector2& va = vertsA[i];
            const Vector2& vb = vertsA[(i + 1) % nA];
            Vector2 edge = vb - va;
            Vector2 axis = Vector2(-edge.y, edge.x); // unnormalized axis
            if (!testAxis(axis)) return manifold; // no collision -> return empty
        }

        // test B edge normals
        const size_t nB = vertsB.size();
        for (size_t i = 0; i < nB; ++i) {
            const Vector2& va = vertsB[i];
            const Vector2& vb = vertsB[(i + 1) % nB];
            Vector2 edge = vb - va;
            Vector2 axis = Vector2(-edge.y, edge.x); // unnormalized axis
            if (!testAxis(axis)) return manifold; // no collision -> return empty
        }

        // polygons intersect
        manifold.hasCollision = true;
        manifold.normal = smallestAxisNormalized;
        manifold.penetration = smallestPenetration;

        // Find incident and reference edges for better contact generation
        struct EdgeInfo {
            size_t index;
            Vector2 normal;
            float dot;
        };

        auto findIncidentEdge = [](const std::vector<Vector2>& verts, const Vector2& refNormal) -> EdgeInfo {
            float minDot = std::numeric_limits<float>::infinity();
            EdgeInfo incident = {0, Vector2(), 0.0f};
            
            const size_t n = verts.size();
            for (size_t i = 0; i < n; ++i) {
                const Vector2& v0 = verts[i];
                const Vector2& v1 = verts[(i + 1) % n];
                Vector2 edge = v1 - v0;
                Vector2 normal = Math::Algebra::normalize(Vector2(-edge.y, edge.x));
                float d = Math::Algebra::dot(normal, refNormal);
                if (d < minDot) {
                    minDot = d;
                    incident.index = i;
                    incident.normal = normal;
                    incident.dot = d;
                }
            }
            return incident;
        };

        // Find edge most aligned with collision normal on both shapes
        EdgeInfo edgeA = findIncidentEdge(vertsA, smallestAxisNormalized);
        EdgeInfo edgeB = findIncidentEdge(vertsB, -smallestAxisNormalized);

        // Get vertices of both edges
        size_t idxA1 = edgeA.index;
        size_t idxA2 = (idxA1 + 1) % vertsA.size();
        size_t idxB1 = edgeB.index;
        size_t idxB2 = (idxB1 + 1) % vertsB.size();

        Vector2 a1 = vertsA[idxA1];
        Vector2 a2 = vertsA[idxA2];
        Vector2 b1 = vertsB[idxB1];
        Vector2 b2 = vertsB[idxB2];

        // Project vertices onto collision normal to find overlap range
        float a1Proj = Math::Algebra::dot(a1, smallestAxisNormalized);
        float a2Proj = Math::Algebra::dot(a2, smallestAxisNormalized);
        float b1Proj = Math::Algebra::dot(b1, smallestAxisNormalized);
        float b2Proj = Math::Algebra::dot(b2, smallestAxisNormalized);

        // Calculate the overlap range
        float minA = std::min(a1Proj, a2Proj);
        float maxA = std::max(a1Proj, a2Proj);
        float minB = std::min(b1Proj, b2Proj);
        float maxB = std::max(b1Proj, b2Proj);

        // Find contact points at overlap boundaries
        float t1 = (maxA - minB) / (maxA - minA);
        float t2 = (minA - maxB) / (minA - maxB);

        // Clamp t values to get points within edge segments
        t1 = std::max(0.0f, std::min(1.0f, t1));
        t2 = std::max(0.0f, std::min(1.0f, t2));

        // Generate contact points
        Vector2 contact1 = a1 + (a2 - a1) * t1;
        Vector2 contact2 = b1 + (b2 - b1) * t2;

        // Add both contact points to manifold
        manifold.addContact(contact1, contact1 - centerA, contact1 - centerB, smallestPenetration);
        manifold.addContact(contact2, contact2 - centerA, contact2 - centerB, smallestPenetration);

        return manifold;
    }
};