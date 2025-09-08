#include "PhysicsServer.hpp"

/* Global */
void PhysicsServer::Update()
{
    CollisionSystem::SpatialGrid->Update();

    for (auto* obj : CollisionSystem::SpatialGrid->Objects) {
        obj->info = Collision2DInfos();
    }

    std::vector<std::pair<Collision2D*, Collision2D*>> pairs = CollisionSystem::SpatialGrid->CollectPhyisicsPair();
    
    for (auto& pair : pairs) {
        CollisionSystem::UpdateCollisionInfos(pair.first, pair.second);
    }
}

void PhysicsServer::Render() {
    CollisionSystem::SpatialGrid->Render();
}

/* Collision System */

void PhysicsServer::CollisionSystem::UpdateCollisionInfos(Collision2D* obj, Collision2D* other) {
    if (other == obj) return;

    Collision2DInfos info = CDA::Detect(obj, other);
    if (info.isColliding) {
        obj->info.isColliding = true;
        obj->info.Colliders.push_back(other);

        if (other->PHYSICS_PARENT) {
            obj->info.MTV.insert_or_assign(other, info.MTV[other]);
            obj->info.ContactPoints.insert_or_assign(other, info.ContactPoints[other]);
            obj->info.PhysicsColliders.push_back(other);
        }
    }
    if (info.isPhysicsColliding) obj->info.isPhysicsColliding = true;
}

/* RigidBody System */

void PhysicsServer::RigidBodySystem::Solve(RigidBody2D* obj, const Collision2DInfos& info)
{
    if (!obj || obj->isStatic || !info.isPhysicsColliding) return;

    for (Collision2D* otherCol : info.PhysicsColliders)
    {
        RigidBody2D* other = dynamic_cast<RigidBody2D*>(otherCol->PHYSICS_PARENT);

        if (other) {
            if (other->isStatic)  
                SolveToStaticBody(obj, dynamic_cast<PhysicsBody2D*>(otherCol->PHYSICS_PARENT), info);
            else 
                SolveToDynamicBody(obj, other, info);
        } 
        else {
            SolveToStaticBody(obj, dynamic_cast<PhysicsBody2D*>(otherCol->PHYSICS_PARENT), info);
        }
    }
}

// ---------------- Solve Static ----------------
void PhysicsServer::RigidBodySystem::SolveToStaticBody(RigidBody2D* obj, PhysicsBody2D* other, const Collision2DInfos& info)
{
    std::vector<glm::vec2> contacts = info.ContactPoints.at(other->collision);
    glm::vec2 mtv = info.MTV.at(other->collision);

    if (glm::length2(mtv) < 1e-8f) return;
    
    float penetration = glm::length(mtv);
    glm::vec2 normal = glm::normalize(mtv);

    // Positional correction
    const float beta = 0.2f;
    const float slop = 0.001f;
    float correctionMagnitude = beta * std::max(penetration - slop, 0.0f);
    obj->transform->position += normal * correctionMagnitude;
    
    for (const auto& globalContact : contacts) 
    {
        glm::vec2 contact = globalContact - obj->transform->position;
        glm::vec2 rPerp = glm::vec2(-contact.y, contact.x);
        glm::vec2 pointVel = obj->linearVelocity + obj->angularVelocity * rPerp;
        
        float velAlongNormal = glm::dot(pointVel, normal);
        if (velAlongNormal > -1e-4f) continue;

        float rCrossN = glm::cross(glm::vec3(contact, 0.0f), glm::vec3(normal, 0.0f)).z;
        float effectiveMass = 1.0f / ((1.0f / obj->mass) + (rCrossN * rCrossN) / obj->getInertia());

        // Restitution

        float e = obj->restitution;
        if (std::abs(velAlongNormal) < 1.0f)
            e = std::lerp(0.0f, e, std::abs(velAlongNormal));

        float j = -(1.0f + e) * velAlongNormal * effectiveMass;
        obj->ApplyImpulse(j * normal, contact);

        // Friction
        glm::vec2 tangentVel = pointVel - normal * velAlongNormal;
        if (glm::length2(tangentVel) > 1e-8f) 
        {
            glm::vec2 tangent = glm::normalize(tangentVel);
            float rCrossT = glm::cross(glm::vec3(contact, 0.0f), glm::vec3(tangent, 0.0f)).z;
            float tangentEffectiveMass = 1.0f / ((1.0f / obj->mass) + (rCrossT * rCrossT) / obj->getInertia());
            float jt = -glm::dot(pointVel, tangent) * tangentEffectiveMass;

            float maxFriction = j * obj->friction;
            jt = glm::clamp(jt, -maxFriction, maxFriction);

            obj->ApplyImpulse(jt * tangent, contact);
        }
    }
}

// --------------- Solve Dynamic ----------------
void PhysicsServer::RigidBodySystem::SolveToDynamicBody(RigidBody2D* obj, RigidBody2D* other, const Collision2DInfos& info)
{
    std::vector<glm::vec2> contacts = info.ContactPoints.at(other->collision);
    glm::vec2 mtv = info.MTV.at(other->collision);

    if (glm::length2(mtv) < 1e-8f) return;
    
    float penetration = glm::length(mtv);
    glm::vec2 normal = glm::normalize(mtv);

    // Positional correction
    const float beta = 0.2f;
    const float slop = 0.001f;
    float correctionMagnitude = beta * std::max(penetration - slop, 0.0f);
    glm::vec2 correction = normal * correctionMagnitude;

    for (const auto& globalContact : contacts) 
    {
        glm::vec2 contactA = globalContact - obj->transform->position;
        glm::vec2 contactB = globalContact - other->transform->position;

        glm::vec2 rPerpA = glm::vec2(-contactA.y, contactA.x);
        glm::vec2 rPerpB = glm::vec2(-contactB.y, contactB.x);
        
        glm::vec2 velA = obj->linearVelocity + obj->angularVelocity * rPerpA;
        glm::vec2 velB = other->linearVelocity + other->angularVelocity * rPerpB;
        glm::vec2 relVel = velA - velB;

        float velAlongNormal = glm::dot(relVel, normal);
        if (velAlongNormal > -1e-4f) continue;

        // Restitution

        float e = std::min(obj->restitution, other->restitution);
        if (std::abs(velAlongNormal) < 1.0f)
            e = std::lerp(0.0f, e, std::abs(velAlongNormal));

        float rCrossNA = glm::cross(glm::vec3(contactA, 0.0f), glm::vec3(normal, 0.0f)).z;
        float rCrossNB = glm::cross(glm::vec3(contactB, 0.0f), glm::vec3(normal, 0.0f)).z;
        
        float effectiveMass = 1.0f / (
            (1.0f / obj->mass) + 
            (1.0f / other->mass) +
            (rCrossNA * rCrossNA) / obj->getInertia() +
            (rCrossNB * rCrossNB) / other->getInertia()
        );

        glm::vec2 impulse = -(1.0f + e) * velAlongNormal * effectiveMass * normal;
        if (other->isSleeping) other->WakeUp();
        obj->ApplyImpulse( impulse, contactA);
        other->ApplyImpulse(-impulse, contactB);

        // Friction
        glm::vec2 tangentVel = relVel - normal * velAlongNormal;
        if (glm::length2(tangentVel) > 1e-8f) 
        {
            glm::vec2 tangent = glm::normalize(tangentVel);

            float rCrossTA = glm::cross(glm::vec3(contactA,0.0f), glm::vec3(tangent,0.0f)).z;
            float rCrossTB = glm::cross(glm::vec3(contactB,0.0f), glm::vec3(tangent,0.0f)).z;

            float tangentEffectiveMass = 1.0f / (
                (1.0f / obj->mass) + 
                (1.0f / other->mass) +
                (rCrossTA*rCrossTA)/obj->getInertia() +
                (rCrossTB*rCrossTB)/other->getInertia()
            );

            float jt = -glm::dot(relVel, tangent) * tangentEffectiveMass;
            float mu = std::sqrt(obj->friction * other->friction);
            jt = glm::clamp(jt, -jt*mu, jt*mu);

            glm::vec2 frictionImpulse = jt * tangent;
            obj->ApplyImpulse( frictionImpulse, contactA);
            other->ApplyImpulse(-frictionImpulse, contactB);
        }
    }

    float totalMass = obj->mass + other->mass;
    float ratioA = other->mass / totalMass;
    float ratioB = obj->mass / totalMass;

    obj->transform->position += correction * ratioA;
    other->transform->position -= correction * ratioB;
}
