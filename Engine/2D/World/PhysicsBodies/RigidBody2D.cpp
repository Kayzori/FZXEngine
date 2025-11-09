#include "RigidBody2D.h"

#include "Math/Algebra/Vector/Utils.h"
#define MATH_INCLUDE_TRIGONOMETRY_UTILS
#include "Math/Utils.h"

// Constructors and Destructors
RigidBody2D::RigidBody2D(
    Collision2D* _collision,
    float _mass,
    float _restitution,
    float _friction,
    float _gravityScale,
    float _linearDamping,
    float _angularDamping
)
    :
    PhysicsBody2D(_collision->getSys(), _collision),
    mass(_mass),
    restitution(_restitution),
    friction(_friction),
    gravityScale(_gravityScale),
    linearDamping(_linearDamping),
    angularDamping(_angularDamping)
{
    linearVelocity = Vector2(0.0f);
    angularVelocity = 0.0f;

    acceleration = Vector2(0.0f);
    angularAcceleration = 0.0f;

    Math::clamp(restitution, 0.0f, 1.0f);
    Math::clamp(friction, 0.0f, 1.0f);

    CalculateInertia();
}

RigidBody2D::RigidBody2D(
    Transform2D* transform,
    Collision2D* _collision,
    float _mass,
    float _restitution,
    float _friction,
    float _gravityScale,
    float _linearDamping,
    float _angularDamping
)
    :
    PhysicsBody2D(transform, _collision->getSys(), _collision),
    mass(_mass),
    restitution(_restitution),
    friction(_friction),
    gravityScale(_gravityScale),
    linearDamping(_linearDamping),
    angularDamping(_angularDamping)
{
    linearVelocity = Vector2(0.0f);
    angularVelocity = 0.0f;

    acceleration = Vector2(0.0f);
    angularAcceleration = 0.0f;

    Math::clamp(restitution, 0.0f, 1.0f);
    Math::clamp(friction, 0.0f, 1.0f);

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
    if (mass <= 0.0f) {
        inertia = 0.0f;
        return;
    }

    std::vector<Vector2> verts = collision->getOVertices();

    float A = 0, Cx = 0, Cy = 0;
    float Ixx = 0, Iyy = 0;
    size_t n = verts.size();

    for (int i = 0; i < n; ++i) {
        const Vector2& p0 = verts[i];
        const Vector2& p1 = verts[(i + 1) % n];

        float cross = Math::Algebra::cross(p0, p1);

        A += cross;
        Cx += (p0.x + p1.x) * cross;
        Cy += (p0.y + p1.y) * cross;

        Ixx += (p0.y * p0.y + p0.y * p1.y + p1.y * p1.y) * cross;
        Iyy += (p0.x * p0.x + p0.x * p1.x + p1.x * p1.x) * cross;
    }

    A *= 0.5;
    Cx /= (6.0f * A);
    Cy /= (6.0f * A);
    Ixx /= 12.0;
    Iyy /= 12.0;

    // Shift to centroid
    float Ixx_c = Ixx - A * Cy * Cy;
    float Iyy_c = Iyy - A * Cx * Cx;

    inertia = (float)((mass / fabs(A)) * fabs(Ixx_c + Iyy_c));
}

float RigidBody2D::getInertia() {return inertia;}

// ------------- Forces & Impulses --------------

void RigidBody2D::ApplyForce(const Vector2 force, const Vector2 point)
{
    // Newton’s 2nd law: F = m * a  =>  a = F / m
    acceleration += force / mass;

    if (point != Vector2(0.0f)) {
        // Torque from force:
        // t = r × F
        float torque = point.x * force.y - point.y * force.x;
        ApplyTorque(torque);
    }
}

void RigidBody2D::ApplyImpulse(const Vector2 impulse, const Vector2 point)
{
    // Impulse: 
    // J = F * dt
    // J = m * a * dt
    // J = m * v

    // Angular impulse:
    // L = r * J

    // v = J / m
    linearVelocity += impulse / mass;

    if (inertia > 0.0f && point != Vector2(0.0f)) {
        // w = L / I
        float angularImpulse = point.x * impulse.y - point.y * impulse.x;
        angularVelocity += angularImpulse / inertia;
    }
}

void RigidBody2D::ApplyTorque(float torque)
{
    // Rotational Newton’s 2nd law: t = I * α => α = t / I
    angularAcceleration += torque / inertia;
}

// ---------------- Integration -----------------
void RigidBody2D::IntegrateForces(float dt)
{
    // Linear velocity update
    // v += a * dt
    linearVelocity += acceleration * dt;

    // Angular velocity update
    // w += α * dt
    angularVelocity += angularAcceleration * dt;

    // Damping (decay)
    // v /= (1 + d * dt)
    // w /= (1 + d * dt)
    linearVelocity *= 1.0f / (1.0f + linearDamping * dt);
    angularVelocity *= 1.0f / (1.0f + angularDamping * dt);

    if (Math::Algebra::length2(linearVelocity) < 1e-4f) 
        linearVelocity = Vector2(0.0f);
    if (fabs(angularVelocity) < 1e-4f)
        angularVelocity = 0.0f;

    // Reset accumulators
    acceleration = Vector2(0.0f);
    angularAcceleration = 0.0f;
}

void RigidBody2D::IntegrateVelocities(float dt)
{
    // Position update
    // x += v * dt
    transform->position += linearVelocity * dt;

    // Rotation update
    // o += w * dt
    transform->rotation += Math::radToDeg(angularVelocity * dt);
    while (transform->rotation >= 360.0) {
        transform->rotation -= 360.0;
    }
}

// --------- RigidBody Physics Updates ----------
void RigidBody2D::MoveAndCollide(float dt)
{
    // Apply gravity force

    IntegrateForces(dt);

    // Solver

    IntegrateVelocities(dt);
}

void RigidBody2D::onUpdate(float dt)
{
    MoveAndCollide(dt);
}
