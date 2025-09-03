#include "RigidBody2D.hpp"
#include <Engine/Servers/PhysicsServer/PhysicsServer.hpp>

// ---------------- Constructor ----------------
RigidBody2D::RigidBody2D(Collision2D* _collision, float _mass, bool _isStatic)
    : PhysicsBody2D(_collision), mass(_mass), isStatic(_isStatic)
{
    restitution = 0.5f;      // bounciness
    friction = 0.4f;         // surface friction
    linearDamping = 0.05f;   // drag
    angularDamping = 0.05f;  // rotational drag

    linearVelocity = glm::vec2(0.0f);
    angularVelocity = 0.0f;

    acceleration = glm::vec2(0.0f);
    torque = 0.0f;

    CalculateInertia();
}

// ---------------- Inertia ----------------
void RigidBody2D::CalculateInertia()
{
    if (isStatic || mass <= 0.0f) {
        inertia = 0.0f;
        return;
    }

    if (collision) {
        if (auto rect = dynamic_cast<Box2D*>(collision->shape)) {
            float w = rect->width;
            float h = rect->height;
            inertia = mass * (w * w + h * h) / 12.0f;
        } else {
            // Generic fallback (e.g., circle)
            inertia = 0.5f * mass;
        }
    } else {
        inertia = 0.5f * mass;
    }
}

// ---------------- Forces & Impulses ----------------
void RigidBody2D::ApplyForce(const glm::vec2 force, const glm::vec2 point)
{
    if (isStatic) return;
    acceleration += force;

    glm::vec2 r = point - transform->position;
    torque += glm::cross(glm::vec3(r, 0.0f), glm::vec3(force, 0.0f)).z;
}

void RigidBody2D::ApplyImpulse(const glm::vec2 impulse, const glm::vec2 point)
{
    if (isStatic) return;

    linearVelocity += impulse / mass;

    if (inertia > 0.0f) {
        glm::vec2 r = point - transform->position;
        float impulseTorque = glm::cross(glm::vec3(r, 0.0f), glm::vec3(impulse, 0.0f)).z;
        angularVelocity += impulseTorque / inertia;
    }
}

// ---------------- Integration ----------------
void RigidBody2D::IntegrateForces(float delta)
{
    if (isStatic) return;

    // Linear
    linearVelocity += acceleration * delta;

    // Angular
    if (inertia > 0.0f) {
        float angularAcc = torque / inertia;
        angularVelocity += angularAcc * delta;
    }

    // Damping (exponential decay for stability)
    linearVelocity *= 1.0f / (1.0f + linearDamping * delta);
    angularVelocity *= 1.0f / (1.0f + angularDamping * delta);

    // Reset accumulators
    acceleration = glm::vec2(0.0f);
    torque = 0.0f;
}

void RigidBody2D::IntegrateVelocities(float delta)
{
    if (isStatic) return;

    transform->position += linearVelocity * delta;
    transform->rotation += angularVelocity * delta;
}

// ---------------- Collision ----------------
void RigidBody2D::ResolveCollision(const Collision2DInfos& info)
{
    if (isStatic || glm::length2(info.MTV) < 1e-6f) return;

    glm::vec2 normal = glm::normalize(info.MTV);

    // Positional correction
    transform->position += info.MTV;

    for (const glm::vec2& point : info.ContactPoints)
    {
        glm::vec2 r = point - transform->position;
        glm::vec2 pointVel = linearVelocity + glm::vec2(-r.y, r.x) * angularVelocity;

        float velAlongNormal = glm::dot(pointVel, normal);
        if (velAlongNormal > 0.0f) continue;

        // --- Restitution (bounce) ---
        float e = restitution;

        // Reflect velocity along normal (classic bounce response)
        linearVelocity = linearVelocity - (1.0f + e) * velAlongNormal * normal;

        // Angular impulse (torque from collision point)
        float denom = 1.0f / mass;
        if (inertia > 0.0f) denom += glm::length2(r) / inertia;

        float j = -(1.0f + e) * velAlongNormal / denom;
        glm::vec2 impulse = j * normal;
        ApplyImpulse(impulse, point);

        // --- Friction ---
        glm::vec2 tangent = pointVel - velAlongNormal * normal;
        if (glm::length2(tangent) > 1e-6f) {
            tangent = glm::normalize(tangent);
            float jt = -glm::dot(pointVel, tangent) / denom;

            float mu = friction;
            glm::vec2 frictionImpulse =
                (std::abs(jt) < j * mu) ? jt * tangent : -j * mu * tangent;

            ApplyImpulse(frictionImpulse, point);
        }
    }
}

// ---------------- Main Update ----------------
void RigidBody2D::MoveAndCollide(float delta)
{
    if (isStatic) return;

    // Apply constant global forces (gravity)
    ApplyForce(Physics2DServer::Gravity * Physics2DServer::GravityDirection);

    // Broadphase / collision detection
    collision->OnUpdate(delta);

    // Integrate forces into velocities
    IntegrateForces(delta);

    // Resolve collisions if needed
    if (collision->info.isPhysicsColliding) {
        ResolveCollision(collision->info);
    }

    // Move by velocity
    IntegrateVelocities(delta);
}

void RigidBody2D::OnUpdate(float delta)
{
    MoveAndCollide(delta);
}
