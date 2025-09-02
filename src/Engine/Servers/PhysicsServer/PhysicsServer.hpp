#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <vector>
#include <glm/glm.hpp>
#include <Engine/Object/2D/Collision2D.hpp>
#include <Engine/Servers/PhysicsServer/QuadTree.hpp>

class PhysicsServer {
public:
    // Init world Quadtree
    static void Init(const glm::vec2& worldCenter, const glm::vec2& worldHalfSize);

    // Collision management
    static void InsertCollision(Collision2D* obj);
    static void DeleteCollision(Collision2D* obj);
    static const std::vector<Collision2D*>& GetCollisions();

    // Collision detection
    static bool Collision2DDetection(Collision2D* obj); // broad + narrow phase
    static bool SATCollision2DDetection(Collision2D* A, Collision2D* B); // narrow phase

private:
    inline static Quadtree tree = Quadtree(0, AABB({0,0},{5000,5000}));
    inline static std::vector<Collision2D*> Collision2DObjects; // all active collisions
};
