#pragma once
#include "PhysicsBody2D.hpp"

class RigidBody2D : public PhysicsBody2D {
public:
    float mass;
    bool isStatic;

    float restitution;   // bounciness [0,1]
    float friction;      // surface friction coefficient
    float linearDamping; // drag
    float angularDamping;

    glm::vec2 linearVelocity;
    float angularVelocity;

    RigidBody2D(Collision2D* collider, float mass = 1.0f, bool isStatic = false);

    // --- Forces & Impulses ---
    void ApplyForce(const glm::vec2 force, const glm::vec2 point = glm::vec2(0.0f));
    void ApplyImpulse(const glm::vec2 impulse, const glm::vec2 point = glm::vec2(0.0f));

    // --- Update steps ---
    void IntegrateForces(float dt);
    void IntegrateVelocities(float dt);
    void ResolveCollision(const Collision2DInfos& info);
    void MoveAndCollide(float delta);

    // --- Update wrapper ---
    void OnUpdate(float dt);

    // --- Getters ---
    bool IsStatic() const { return isStatic; }

private:
    void CalculateInertia();

public:
    float inertia;
    glm::vec2 acceleration;
    float torque;
};
