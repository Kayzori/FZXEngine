#pragma once
#include "PhysicsBody2D.hpp"

class RigidBody2D : public PhysicsBody2D {
public:
    float mass;
    bool isStatic;

    float restitution;      // bounciness [0,1]
    float friction;         // surface friction coefficient [0, 1]

    glm::vec2 linearVelocity;
    float linearDamping;    // drag
    float angularVelocity;
    float angularDamping;   // angular drag

    RigidBody2D(
        Collision2D* _collision,
        float _mass = 1.0f,
        float _restitution = 0.2f,
        float _friction = 0.4f,
        float _linearDamping = 0.01f,
        float _angularDamping = 0.01f,
        bool _isStatic = false);

    // --- Forces & Impulses ---
    void ApplyForce(const glm::vec2 force, const glm::vec2 point = glm::vec2(0.0f));
    void ApplyTorque(const float torque);
    void ApplyImpulse(const glm::vec2 impulse, const glm::vec2 point = glm::vec2(0.0f));

    // --- Update steps ---
    void IntegrateForces(float dt);
    void IntegrateVelocities(float dt);
    
    virtual void ResolveCollision(const Collision2DInfos& info);
    void MoveAndCollide(float delta);

    // --- Update wrapper ---
    void OnUpdate(float dt);

    // --- Getters ---
    bool IsStatic() const { return isStatic; }

private:
    void CalculateInertia();

    float inertia;
    glm::vec2 acceleration;
    float angularAcceleration;
};
