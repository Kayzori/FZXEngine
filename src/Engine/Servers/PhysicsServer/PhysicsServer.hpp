#pragma once

#include <vector>
#include <Math/Math.hpp>
#include <Engine/Object/2D/Collision2D.hpp>
#include "QuadTree.hpp"

// SERVER
class Physics2DServer {
public:
    // Consts
    inline static float Gravity = 980.0f;
    inline static glm::vec2 GravityDirection = {0, 1};

    // Systems
    class CollisionSystem
    {
    public:
        // Init board for QuadTree
        static void InitCollisionBoard(AABB board);

        // Collision management
        static void InsertCollision(Collision2D* obj);
        static void DeleteCollision(Collision2D* obj);
        static const std::unordered_set<Collision2D*>& GetCollisions();

        // Collision Detection With Neighbors
        // QuadTree To Get Neighbors
        // Uses Base Algorithm To Update Infos
        static void UpdateCollisionInfos(Collision2D* obj);

        // Base Collision Detection Algorithm
        // AABB + SAT Collision Detection
        static Collision2DInfos CollisionDetection(Collision2D* A, Collision2D* B);
    private:
        inline static QuadTree tree = QuadTree(0, {0.0f, 0.0f, 0.0f, 0.0f});
        inline static std::unordered_set<Collision2D*> Collision2DObjects;
    };
};
