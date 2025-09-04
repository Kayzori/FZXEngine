#include "RigidBody2D.hpp"
#include <Engine/Servers/PhysicsServer/PhysicsServer.hpp>

// ---------------- Constructor ----------------
RigidBody2D::RigidBody2D(
    Collision2D* _collision,
    float _mass,
    float _restitution,
    float _friction,
    float _linearDamping,
    float _angularDamping,
    bool _isStatic
)
    :
    PhysicsBody2D(_collision),
    mass(_mass),
    restitution(_restitution),
    friction(_friction),
    linearDamping(_linearDamping),
    angularDamping(_angularDamping),
    isStatic(_isStatic)
{
    linearVelocity = glm::vec2(0.0f);
    angularVelocity = 0.0f;

    acceleration = glm::vec2(0.0f);
    angularAcceleration = 0.0f;

    glm::clamp(restitution, 0.0f, 1.0f);
    glm::clamp(friction, 0.0f, 1.0f);

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
        // Rectangle inertia around center of mass:
        // I = (1/12) * m * (w² + h²)
        if (Box2D* box = dynamic_cast<Box2D*>(collision->shape)) {
            float w = box->w;
            float h = box->h;
            inertia = mass * (w * w + h * h) / 12.0f;
        }
        // Circle inertia:
        // I = (1/2) * m * r²
        else if (Circle2D* circle = dynamic_cast<Circle2D*>(collision->shape)) {
            inertia = 0.5f * mass * (circle->radius * circle->radius);
        }
    }
}

// ---------------- Forces & Impulses ----------------
void RigidBody2D::ApplyForce(const glm::vec2 force, const glm::vec2 point)
{
    // Newton’s 2nd law: F = m * a  =>  a = F / m
    if (isStatic) return;
    acceleration += force / mass;

    if (point != glm::vec2(0.0f)) {
        // Torque from force:
        // t = r × F
        float torque = point.x * force.y - point.y * force.x;
        ApplyTorque(torque);
    }
}

void RigidBody2D::ApplyImpulse(const glm::vec2 impulse, const glm::vec2 point)
{
    // Impulse-momentum: 
    // J = F * dt
    // J = m * a * dt
    // J = m * v

    // Angular impulse:
    // L = r * J

    if (isStatic) return;

    // v = J / m
    linearVelocity += impulse / mass;

    if (inertia > 0.0f && point != glm::vec2(0.0f)) {
        // w = L / I
        float angularImpulse = point.x * impulse.y - point.y * impulse.x;
        angularVelocity += angularImpulse / inertia;
    }
}

void RigidBody2D::ApplyTorque(float torque)
{
    // Rotational Newton’s 2nd law: t = I * α => α = t / I
    if (isStatic || inertia <= 0.0f) return;
    angularAcceleration += torque / inertia;
}

// ---------------- Integration ----------------
void RigidBody2D::IntegrateForces(float delta)
{
    if (isStatic) return;

    // Linear velocity update
    // v += a * dt
    linearVelocity += acceleration * delta;

    // Angular velocity update
    // w += α * dt
    angularVelocity += angularAcceleration * delta;

    // Damping (decay)
    // v /= (1 + d * dt)
    // w /= (1 + d * dt)
    linearVelocity *= 1.0f / (1.0f - linearDamping * delta);
    angularVelocity *= 1.0f / (1.0f - angularDamping * delta);

    if (glm::length2(linearVelocity) < 1e-4f) 
        linearVelocity = glm::vec2(0.0f);
    if (std::abs(angularVelocity) < 1e-4f)
        angularVelocity = 0.0f;

    // Reset accumulators
    acceleration = glm::vec2(0.0f);
    angularAcceleration = 0.0f;
}

void RigidBody2D::IntegrateVelocities(float delta)
{
    if (isStatic) return;

    // Position update
    // x += v * dt
    transform->position += linearVelocity * delta;

    // Rotation update
    // O += w * dt
    transform->rotation += rad2deg(angularVelocity * delta);
}

// ---------------- Collision ----------------
void RigidBody2D::ResolveCollision(const Collision2DInfos& info)
{
    if (isStatic || glm::length2(info.MTV) < 1e-6f) return;

    glm::vec2 normal = glm::normalize(info.MTV);

    // --- Positional correction (Baumgarte stabilization) ---
    const float percent = 0.8f;   // usually 20-80%
    const float slop = 0.01f;     // penetration tolerance
    float penetration = glm::length(info.MTV);
    glm::vec2 correction = percent * std::max(penetration - slop, 0.0f) * normal;
    transform->position += correction;
    
    std::cout << "Contact Points: \n" << info.ContactPoints << '\n';
    glm::vec2 GlobalContactPoint = info.getGlobalContactPoint(info.ContactPoints);
    std::cout << "Global Contact Point: \n" << GlobalContactPoint << '\n';

    // --- Solve impulses at GlobalContactPoint ---

    // Velocity at GlobalContactPoint:
    // v_point = v_linear + w × r
    glm::vec2 pointVel = linearVelocity + angularVelocity * glm::vec2(-GlobalContactPoint.y, GlobalContactPoint.x);

    // Relative velocity along normal
    float velAlongNormal = glm::dot(pointVel, normal);
    if (velAlongNormal > 0.0f) return; // separating

    // Restitution (bounce)
    float e = restitution;
    e *= glm::clamp(std::abs(velAlongNormal) / 100.0f, 0.0f, 1.0f);
    if (std::abs(velAlongNormal) < 100.0f) e = 0.0f;

    // Effective mass denominator
    float cn = GlobalContactPoint.x * normal.y - GlobalContactPoint.y * normal.x;
    float denom = (1.0f / mass) + (cn * cn) / inertia;

    // Normal impulse
    float j = -(1.0f + e) * velAlongNormal / denom;
    glm::vec2 impulse = j * normal;
    std::cout << "Bounce Impulse Applied: " << impulse << '\n';
    ApplyImpulse(impulse, GlobalContactPoint);

    // --- Friction impulse ---
    glm::vec2 tangent = pointVel - velAlongNormal * normal;
    if (glm::length2(tangent) > 1e-6f)
    {
        tangent = glm::normalize(tangent);

        float ct = GlobalContactPoint.x * tangent.y - GlobalContactPoint.y * tangent.x;
        float denomFriction = (1.0f / mass) + (ct * ct) / inertia;

        // Calculate tangential velocity
        float velAlongTangent = glm::dot(pointVel, tangent);
        float jt = -velAlongTangent / denomFriction;

        // Apply Coulomb's friction law
        float mu = friction;
        glm::vec2 frictionImpulse;
        if (std::abs(jt) <= j * mu)
            frictionImpulse = jt * tangent;
        else
            frictionImpulse = -j * mu * glm::sign(velAlongTangent) * tangent;
        std::cout << "Friction Impulse Applied: " << frictionImpulse << "\n\n\n\n\n";
        if (glm::length2(frictionImpulse) > 1e-6f)
            ApplyImpulse(frictionImpulse, GlobalContactPoint);
    }
}

// ---------------- Main Update ----------------
void RigidBody2D::MoveAndCollide(float delta)
{
    ApplyForce(Physics2DServer::Gravity * Physics2DServer::GravityDirection * mass);

    collision->OnUpdate(delta);

    IntegrateForces(delta);

    if (collision->info.isPhysicsColliding) {
        ResolveCollision(collision->info);
    }

    IntegrateVelocities(delta);
}

void RigidBody2D::OnUpdate(float delta)
{
    MoveAndCollide(delta);
}
