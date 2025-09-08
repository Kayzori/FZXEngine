#include "CollisionDetectionAlgorithm.hpp"

// --------------------- Circle Circle Collision Detection --------------------
Collision2DInfos CDA::CCCD(Collision2D* A, Collision2D* B) {
    Collision2DInfos info;

    Circle2D* circleA = static_cast<Circle2D*>(A->shape);
    Circle2D* circleB = static_cast<Circle2D*>(B->shape);
    if (!circleA || !circleB) return info;

    glm::vec2 delta = B->transform->position - A->transform->position;
    float distSq = glm::dot(delta, delta);

    float totalRadius = circleA->radius + circleB->radius;
    float totalRadiusSq = totalRadius * totalRadius;

    if (distSq > totalRadiusSq) return info; // No collision

    info.isColliding = true;

    if (A->PHYSICS_PARENT && B->PHYSICS_PARENT) {
        info.isPhysicsColliding = true;

        // Approximate fast sqrt when needed
        float dist = (distSq > 1e-8f) ? std::sqrt(distSq) : 0.0f;
        glm::vec2 normal = (dist > 1e-8f) ? delta * (1.0f / dist) : glm::vec2(1.0f, 0.0f);

        float penetration = totalRadius - dist;

        info.MTV[B] = -normal * penetration;

        // Contact point halfway between overlap
        glm::vec2 contact = A->transform->position + normal * (circleA->radius - penetration * 0.5f);
        info.ContactPoints[B].push_back(contact);
    }

    return info;
}

// --------------------- Circle Polygon Collision Detection -------------------
Collision2DInfos CDA::CPCD(Collision2D* circleObj, Collision2D* polyObj, Collision2D* ReferenceObj) {
    Collision2DInfos info;

    Circle2D* circle = static_cast<Circle2D*>(circleObj->shape);
    if (!circle) return info;

    const auto& verts = polyObj->getVertices();
    if (verts.size() < 3) return info;

    glm::vec2 circleCenter = circleObj->transform->position;

    float minDistSq = std::numeric_limits<float>::max();
    glm::vec2 closestPoint(0.0f);
    glm::vec2 normal(0.0f);

    auto closestPointOnSegment = [](const glm::vec2& p, const glm::vec2& a, const glm::vec2& b) {
        glm::vec2 ab = b - a;
        float denom = glm::dot(ab, ab);
        if (denom <= 1e-12f) return a; // Degenerate edge
        float t = glm::dot(p - a, ab) / denom;
        t = (t < 0.0f) ? 0.0f : ((t > 1.0f) ? 1.0f : t);
        return a + t * ab;
    };

    // Fastest: use squared distance (no sqrt until last moment)
    for (size_t i = 0, n = verts.size(); i < n; ++i) {
        const glm::vec2& v1 = verts[i];
        const glm::vec2& v2 = verts[(i + 1) % n];

        glm::vec2 pointOnEdge = closestPointOnSegment(circleCenter, v1, v2);
        glm::vec2 diff = circleCenter - pointOnEdge;
        float distSq = glm::dot(diff, diff);

        if (distSq < minDistSq) {
            minDistSq = distSq;
            closestPoint = pointOnEdge;
            normal = (distSq > 1e-12f) ? (diff * (1.0f / std::sqrt(distSq))) : glm::vec2(1.0f, 0.0f);
        }
    }

    bool inside = polyObj->hasPoint(circleCenter);

    float radius = circle->radius;
    if (inside || minDistSq <= radius * radius) {
        info.isColliding = true;

        if (circleObj->PHYSICS_PARENT && polyObj->PHYSICS_PARENT) {
            info.isPhysicsColliding = true;

            float minDist = std::sqrt(minDistSq);
            float penetration = inside ? radius + minDist : radius - minDist;

            info.MTV[ReferenceObj] = normal * penetration * ((ReferenceObj != circleObj) ? 1.0f : -1.0f);

            glm::vec2 contact = circleCenter - normal * radius;
            info.ContactPoints[ReferenceObj].push_back(contact);
        }
    }

    return info;
}

// -------------------- Polygon Polygon Collision Detection--------------------
static int orientation(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c){
    float val = (b.y - a.y) * (c.x - b.x) - (b.x - a.x) * (c.y - b.y);
    if(std::abs(val) < 1e-8f) return 0;
    return (val > 0) ? 1 : 2;
}

static bool onSegment(const glm::vec2& a, const glm::vec2& c, const glm::vec2& b){
    return std::min(a.x, b.x) <= c.x && c.x <= std::max(a.x, b.x) &&
           std::min(a.y, b.y) <= c.y && c.y <= std::max(a.y, b.y);
}

static bool EdgeIntersection(const Edge2D& e1, const Edge2D& e2, glm::vec2& contactPoint) {
    const glm::vec2& p1 = e1.p1, &q1 = e1.p2;
    const glm::vec2& p2 = e2.p1, &q2 = e2.p2;

    // Early AABB rejection
    if (std::max(p1.x, q1.x) < std::min(p2.x, q2.x)) return false;
    if (std::min(p1.x, q1.x) > std::max(p2.x, q2.x)) return false;
    if (std::max(p1.y, q1.y) < std::min(p2.y, q2.y)) return false;
    if (std::min(p1.y, q1.y) > std::max(p2.y, q2.y)) return false;

    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    // General intersection
    if (o1 != o2 && o3 != o4) {
        // Calculate intersection point
        float denom = (p1.x - q1.x) * (p2.y - q2.y) - (p1.y - q1.y) * (p2.x - q2.x);
        if (std::abs(denom) < 1e-8f) return false;
        
        float t = ((p1.x - p2.x) * (p2.y - q2.y) - (p1.y - p2.y) * (p2.x - q2.x)) / denom;
        contactPoint = p1 + t * (q1 - p1);
        return true;
    }

    // Collinear cases
    if (o1 == 0 && onSegment(p1, p2, q1)) { contactPoint = p2; return true; }
    if (o2 == 0 && onSegment(p1, q2, q1)) { contactPoint = q2; return true; }
    if (o3 == 0 && onSegment(p2, p1, q2)) { contactPoint = p1; return true; }
    if (o4 == 0 && onSegment(p2, q1, q2)) { contactPoint = q1; return true; }

    return false;
}

Collision2DInfos CDA::PPCD(Collision2D* A, Collision2D* B) {
    Collision2DInfos info;

    const auto& vertsA = A->getVertices();
    const auto& vertsB = B->getVertices();
    if (vertsA.size() < 3 || vertsB.size() < 3) return info;

    float minOverlap = std::numeric_limits<float>::max();
    glm::vec2 bestAxis(0.0f);

    auto project_on_axis = [](const std::vector<glm::vec2>& verts, const glm::vec2& axis) -> std::pair<float, float> {
        float minVal = glm::dot(axis, verts[0]);
        float maxVal = minVal;
        for (size_t i = 1; i < verts.size(); ++i) {
            float p = glm::dot(axis, verts[i]);
            minVal = std::min(minVal, p);
            maxVal = std::max(maxVal, p);
        }
        return {minVal, maxVal};
    };

    auto edge_axis = [&](const glm::vec2& a, const glm::vec2& b) -> glm::vec2 {
        glm::vec2 e = b - a;
        glm::vec2 axis(-e.y, e.x);
        float len = glm::length(axis);
        if (len < 1e-8f) return glm::vec2(0.0f);
        return axis / len;
    };

    // ---- SAT check on edges of A ----
    for (size_t i = 0; i < vertsA.size(); ++i) {
        size_t j = (i + 1) % vertsA.size();
        glm::vec2 axis = edge_axis(vertsA[i], vertsA[j]);
        if (glm::length(axis) < EPS) continue;

        auto [minA, maxA] = project_on_axis(vertsA, axis);
        auto [minB, maxB] = project_on_axis(vertsB, axis);

        if (maxA < minB || maxB < minA) return info; // early out

        float overlap = std::min(maxA, maxB) - std::max(minA, minB);
        if (overlap < minOverlap) {
            minOverlap = overlap;
            bestAxis = axis;
        }
    }

    // ---- SAT check on edges of B ----
    for (size_t i = 0; i < vertsB.size(); ++i) {
        size_t j = (i + 1) % vertsB.size();
        glm::vec2 axis = edge_axis(vertsB[i], vertsB[j]);
        if (glm::length(axis) < EPS) continue;

        auto [minA, maxA] = project_on_axis(vertsA, axis);
        auto [minB, maxB] = project_on_axis(vertsB, axis);

        if (maxA < minB || maxB < minA) return info; // early out

        float overlap = std::min(maxA, maxB) - std::max(minA, minB);
        if (overlap < minOverlap) {
            minOverlap = overlap;
            bestAxis = axis;
        }
    }

    // If no separating axis: collision confirmed
    info.isColliding = true;

    if (A->PHYSICS_PARENT && B->PHYSICS_PARENT) {
        info.isPhysicsColliding = true;

        glm::vec2 centerA = A->getCenter();
        glm::vec2 centerB = B->getCenter();

        if (glm::dot(bestAxis, centerB - centerA) < 0.0f) bestAxis = -bestAxis;
        info.MTV[B] = -bestAxis * minOverlap;

        std::vector<glm::vec2> contacts;

        // 1) Fast: vertices inside the other polygon
        for (const auto& v : vertsA) {
            if (B->hasPoint(v)) {
                contacts.push_back(v);
                if (contacts.size() >= 2) break;
            }
        }
        for (const auto& v : vertsB) {
            if (A->hasPoint(v)) {
                contacts.push_back(v);
                if (contacts.size() >= 2) break;
            }
        }

        // 2) Edge intersections if not enough contacts
        if (contacts.size() < 2) {
            for (auto& edgeA : A->getEdges()) {
                for (auto& edgeB : B->getEdges()) {
                    glm::vec2 c;
                    if (EdgeIntersection(edgeA, edgeB, c)) {
                        bool duplicate = false;
                        for (const auto& existing : contacts) {
                            if (glm::length2(c - existing) < EPS * EPS) {
                                duplicate = true;
                                break;
                            }
                        }
                        if (!duplicate) {
                            contacts.push_back(c);
                            if (contacts.size() >= 2) break;
                        }
                    }
                }
                if (contacts.size() >= 2) break;
            }
        }

        // 3) Final fallback: use midpoint of centers
        if (contacts.empty()) {
            contacts.push_back((centerA + centerB) * 0.5f);
        }

        // Store only up to 2 contacts
        for (const auto& c : contacts) {
            info.ContactPoints[B].push_back(c);
            if (info.ContactPoints[B].size() >= 2) break;
        }
    }

    return info;
}

// ----------------------- Global Collision Detection -------------------------
Collision2DInfos CDA::Detect(Collision2D* A, Collision2D* B) {
    const auto& vertsA = A->getVertices();
    const auto& vertsB = B->getVertices();
    if (vertsA.empty() || vertsB.empty()) return Collision2DInfos();

    Circle2D* circleA = dynamic_cast<Circle2D*>(A->shape);
    Circle2D* circleB = dynamic_cast<Circle2D*>(B->shape);

    if (circleA && circleB) return CCCD(A, B);
    if (circleA && vertsB.size() >= 3) return CPCD(A, B, B);
    if (circleB && vertsA.size() >= 3) return CPCD(B, A, B);
    return PPCD(A, B);
}


