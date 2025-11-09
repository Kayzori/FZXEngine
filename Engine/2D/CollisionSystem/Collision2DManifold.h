#pragma once

#include "Math/Algebra/Vector/Vector2.h"

#include <vector>

// Forward declaration
class Collision2D;

struct ContactPoint2D {
    Vector2 position;       // World space contact point
    Vector2 localA;         // Contact point in A's local space
    Vector2 localB;         // Contact point in B's local space
    float penetration;      // Penetration depth at this point
};

class Collision2DManifold {
public:
    Collision2DManifold() = default;
    Collision2DManifold(Collision2D* bodyA, Collision2D* bodyB) 
        : bodyA(bodyA), bodyB(bodyB) {}

    // Bodies involved in collision
    Collision2D* bodyA = nullptr;
    Collision2D* bodyB = nullptr;

    // Contact information
    bool hasCollision = false;
    std::vector<ContactPoint2D> contacts;  // Contact points (usually 1-2 points)
    Vector2 normal;                        // Contact normal (points from A to B)
    float penetration = 0.0f;              // Maximum penetration depth

    // Helper methods
    void clear() {
        hasCollision = false;
        contacts.clear();
        normal = Vector2();
        penetration = 0.0f;
    }

    void addContact(const Vector2& worldPoint, const Vector2& localA, 
                   const Vector2& localB, float penetration) {
        contacts.push_back({worldPoint, localA, localB, penetration});
    }
};