#include "RigidBody2D.hpp"
#include <Engine/Servers/PhysicsServer/PhysicsServer.hpp>

// ---------------- Constructor ----------------
RigidBody2D::RigidBody2D(
    Collision2D* _collision,
    float _mass,
    float _restitution,
    float _friction,
    float _gravityScale,
    float _linearDamping,
    float _angularDamping,
    bool _canSleep,
    bool _isSleeping,
    bool _isStatic
)
    :
    PhysicsBody2D(_collision),
    mass(_mass),
    restitution(_restitution),
    friction(_friction),
    gravityScale(_gravityScale),
    linearDamping(_linearDamping),
    angularDamping(_angularDamping),
    canSleep(_canSleep),
    isSleeping(_isSleeping),
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

/*
x: position
o: rotation
v: linear velocity
w: angular velocity
a: acceleration
α: anuglar acceleration
J: linear impulse
L: angular impulse
I: interia
m: mass
F: force
t: torque
r: distance between center and force application (radius)
d: damping
*/

// ----------------- Inertia --------------------
void RigidBody2D::CalculateInertia()
{
    if (isStatic || mass <= 0.0f) {
        inertia = 0.0f;
        return;
    }

    if (collision) {
        // Rectangle: I = (1/12) * m * (w² + h²)
        if (Box2D* box = dynamic_cast<Box2D*>(collision->shape)) {
            float w = box->w;
            float h = box->h;
            inertia = mass * (w * w + h * h) / 12.0f;
        }
        // Circle inertia: I = (1/2) * m * r²
        else if (Circle2D* circle = dynamic_cast<Circle2D*>(collision->shape)) {
            inertia = 0.5f * mass * (circle->radius * circle->radius);
        }
    }
}

float RigidBody2D::getInertia() {return inertia;}

// ------------- Forces & Impulses --------------

void RigidBody2D::WakeUp()
{
    std::cout << "Waky Waky" << std::endl; // IDK it seems cool for debugging :)
    isSleeping = false;
}

void RigidBody2D::ApplyForce(const glm::vec2 force, const glm::vec2 point)
{
    // Newton’s 2nd law: F = m * a  =>  a = F / m
    if (isStatic || isSleeping) return;
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
    // Impulse: 
    // J = F * dt
    // J = m * a * dt
    // J = m * v

    // Angular impulse:
    // L = r * J

    if (isStatic || isSleeping) return;

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
    if (isStatic || isSleeping || inertia <= 0.0f) return;
    angularAcceleration += torque / inertia;
}

// ---------------- Integration -----------------
void RigidBody2D::IntegrateForces(float delta)
{
    if (isStatic || isSleeping) return;

    // Linear velocity update
    // v += a * dt
    linearVelocity += acceleration * delta;

    // Angular velocity update
    // w += α * dt
    angularVelocity += angularAcceleration * delta;

    // Damping (decay)
    // v /= (1 + d * dt)
    // w /= (1 + d * dt)
    linearVelocity *= 1.0f / (1.0f + linearDamping * delta);
    angularVelocity *= 1.0f / (1.0f + angularDamping * delta);

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
    if (isStatic || isSleeping) return;

    // Position update
    // x += v * dt
    transform->position += linearVelocity * delta;

    // Rotation update
    // o += w * dt
    transform->rotation += rad2deg(angularVelocity * delta);
    while (transform->rotation >= 360.0) {
        transform->rotation -= 360.0;
    }
}

// TODO
void RigidBody2D::checkSleep()
{
    if (!canSleep) return;
    if (glm::length2(linearVelocity) < sleepLinearThreshold * sleepLinearThreshold &&
        std::abs(angularVelocity) < sleepAngularThreshold) 
    {
        std::cout << "sleeps" << std::endl;
        isSleeping = true;
        linearVelocity = glm::vec2(0.0f);
        angularVelocity = 0.0f;
    }
}

// --------- RigidBody Physics Updates ----------
void RigidBody2D::MoveAndCollide(float delta)
{
    if (isStatic || isSleeping) return;

    ApplyForce(PhysicsServer::Gravity * PhysicsServer::GravityDirection * mass * gravityScale);
    
    IntegrateForces(delta);

    if (collision->info.isPhysicsColliding) {
        PhysicsServer::RigidBodySystem::Solve(this, collision->info);
    }

    IntegrateVelocities(delta);
}

// GameLoop
void RigidBody2D::OnUpdate(float delta)
{
    MoveAndCollide(delta);
}
