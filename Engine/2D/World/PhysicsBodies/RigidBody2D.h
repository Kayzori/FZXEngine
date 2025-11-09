#pragma once

#include "PhysicsBody2D.h"

class RigidBody2D : public PhysicsBody2D {
public:
    // Properties
    float mass;

    float restitution;      // bounciness [0,1]
    float friction;         // surface friction coefficient [0, 1]
    float gravityScale;

    Vector2 linearVelocity;
    float linearDamping;    // drag
    float angularVelocity;
    float angularDamping;   // angular drag

    RigidBody2D(
        Collision2D* _collision,
        float _mass = 1.0f,
        float _restitution = 0.2f,
        float _friction = 0.4f,
        float _gravityScale = 1.0f,
        float _linearDamping = 0.01f,
        float _angularDamping = 0.01f
    );
    RigidBody2D(
        Transform2D* transform,
        Collision2D* _collision,
        float _mass = 1.0f,
        float _restitution = 0.2f,
        float _friction = 0.4f,
        float _gravityScale = 1.0f,
        float _linearDamping = 0.01f,
        float _angularDamping = 0.01f
    );
    ~RigidBody2D() = default;

    // Methods

    // --- Forces & Impulses ---
    void ApplyForce(const Vector2 force, const Vector2 point = Vector2(0.0f));
    void ApplyTorque(const float torque);
    void ApplyImpulse(const Vector2 impulse, const Vector2 point = Vector2(0.0f));

    // --- Update steps ---
    void IntegrateForces(float dt);
    void IntegrateVelocities(float dt);
    
    // --- Gets Functions --
    float getInertia();
    void MoveAndCollide(float dt);

    void onUpdate(float dt) override;

private:
    void CalculateInertia();

    float inertia;
    Vector2 acceleration;
    float angularAcceleration;
};