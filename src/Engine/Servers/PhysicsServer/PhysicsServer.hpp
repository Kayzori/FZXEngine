#pragma once

#include <vector>
#include <Math/Math.hpp>
#include <Engine/Object/2D/Collision2D.hpp>
#include <Engine/Object/2D/PhysicsBody2D/RigidBody2D.hpp>
#include "Algorithms/CollisionDetectionAlgorithm.hpp"
#include "CollisionSpatialGrid.hpp"

// SERVER
class PhysicsServer {
public:
    // Consts
    inline static float Gravity = 980.0f;
    inline static glm::vec2 GravityDirection = {0, 1};

    static void Update();
    static void Render();

    // Systems
    class CollisionSystem
    {
    public:
        inline static CollisionSpatialGrid* SpatialGrid = nullptr;
        static void UpdateCollisionInfos(Collision2D* obj, Collision2D* other);
    };

    class RigidBodySystem {
    public:
        // Solver
        static void Solve(RigidBody2D* obj, const Collision2DInfos& info);
    private:
        static void SolveToStaticBody(RigidBody2D* obj, PhysicsBody2D* other, const Collision2DInfos& info);
        static void SolveToDynamicBody(RigidBody2D* obj, RigidBody2D* other, const Collision2DInfos& info);
    };
};
