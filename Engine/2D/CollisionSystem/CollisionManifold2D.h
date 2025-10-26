#pragma once

#include "../Object2D.h"

struct CollisionManifold2D {
    // Collision State
    bool colliding = false;

    // References to the two colliding bodies
    void* A = nullptr;
    void* B = nullptr;

    // Geometry Info (from collision detection)
    Vector2 normal;                     // Collision normal (points from A -> B)
    float penetration = 0.0f;           // Penetration depth
    Vector2 contacts[2];                // Up to 2 contact points for 2D
    int contactCount = 0;

    // Physical Properties (from both bodies)
    float restitution = 0.0f;           // Bounciness (avg of both)
    float staticFriction = 0.0f;
    float dynamicFriction = 0.0f;

    // Solver Cache (used by iterative solver)
    float accumulatedNormalImpulse[2] = { 0.0f, 0.0f };
    float accumulatedTangentImpulse[2] = { 0.0f, 0.0f };

    // Bias term to prevent sinking
    float restitutionBias = 0.0f;

    // Utility
    void reset() {
        colliding = false;
        A = nullptr;
        B = nullptr;
        penetration = 0.0f;
        contactCount = 0;
        restitution = staticFriction = dynamicFriction = 0.0f;
        restitutionBias = 0.0f;
    }
};
