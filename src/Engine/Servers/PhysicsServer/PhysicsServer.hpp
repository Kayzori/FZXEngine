#pragma once

#include <vector>
#include <Math/Math.hpp>
#include <Engine/Object/2D/Collision2D.hpp>
#include <Engine/Object/2D/PhysicsBody2D/RigidBody2D.hpp>
#include "CollisionQuadTree.hpp"
#include "Algorithms/CollisionDetectionAlgorithm.hpp"

// SERVER
class PhysicsServer {
public:
    // Consts
    inline static float Gravity = 980.0f;
    inline static glm::vec2 GravityDirection = {0, 1};

    // Systems
    class CollisionSystem
    {
    public:
        static void InitCollisionBoard(AABB board);
        static void RenderCollisionBoard();
        static void UpdateCollisionBoard();

        static void InsertCollision(Collision2D* obj);
        static void DeleteCollision(Collision2D* obj);
        static const std::unordered_set<Collision2D*>& GetCollisions();

        static void UpdateCollisionInfos(Collision2D* obj);

    private:
        inline static CollisionQuadTree tree = CollisionQuadTree(0, AABB({0.0f, 0.0f, 0.0f, 0.0f}));
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
