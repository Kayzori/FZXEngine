#include "Collision2DSystem.h"
#include "NarrowPhase/NarrowPhase.h"
#include "Math/Geometry2D/Core/AABB2D.h"
#include <algorithm>

// Board-Phase methods

void Collision2DSystem::addObject(Collision2D* obj)
{
    objects.push_back(obj);
}

void Collision2DSystem::removeObject(Collision2D* obj)
{
    objects.erase(std::remove(objects.begin(), objects.end(), obj), objects.end());
}

std::vector<Collision2D*> Collision2DSystem::query(const AABB2D& aabb) const
{
#ifdef FZX_2D_COLLISION_SYSTEM_USE_UNIFORM_GRID
    return grid->queryNearby(aabb);
#else
    std::vector<Collision2D*> result;
    for (Collision2D* obj : objects) {
        if (obj) {
            const AABB2D objAABB = AABB2D(obj->getVertices());
            if (aabb.intersects(objAABB)) {
                result.push_back(obj);
            }
        }
    }
    return result;
#endif
}

std::vector<std::pair<Collision2D*, Collision2D*>> Collision2DSystem::computePairs() const
{
#ifdef FZX_2D_COLLISION_SYSTEM_USE_UNIFORM_GRID
    return grid->computePairs();
#else
    std::vector<std::pair<Collision2D*, Collision2D*>> pairs;
    for (size_t i = 0; i < objects.size(); ++i) {
        for (size_t j = i + 1; j < objects.size(); ++j) {
            if (objects[i] && objects[j]) {
                const AABB2D aabb1 = AABB2D(objects[i]->getVertices());
                const AABB2D aabb2 = AABB2D(objects[j]->getVertices());
                if (aabb1.intersects(aabb2)) {
                    pairs.emplace_back(objects[i], objects[j]);
                }
            }
        }
    }
    return pairs;
#endif
}

// Narrow-Phase methods

void Collision2DSystem::updateManifolds()
{
    manifolds.clear();
    col_manifolds.clear();
    ncol_manifolds.clear();

    // Broad-phase: compute candidate pairs
    const auto pairs = computePairs();

    for (const auto& pr : pairs) {
        Collision2D* A = pr.first;
        Collision2D* B = pr.second;
        if (!A || !B) continue;

        // Dispatch to appropriate narrow-phase test
        Collision2DManifold man(A, B);

        if (A->p_shape == PrimitiveShape2D::CONVEX_POLYGON2D && B->p_shape == PrimitiveShape2D::CONVEX_POLYGON2D)
            man = NarrowPhase::SAT(A, B);
        else if (A->p_shape != PrimitiveShape2D::CONVEX_POLYGON2D && B->p_shape != PrimitiveShape2D::CONVEX_POLYGON2D)
            man = NarrowPhase::CircleXCircle(A, B);
        else
            man = NarrowPhase::CircleXPolygon(A, B);

        manifolds.push_back(man);

        if (man.hasCollision)
            col_manifolds.push_back(man);
        else
            ncol_manifolds.push_back(man);
    }
}

const std::vector<Collision2DManifold>& Collision2DSystem::getManifolds() const
{
    return manifolds;
}

const std::vector<Collision2DManifold>& Collision2DSystem::getCollisionsManifolds() const
{
    return col_manifolds;
}

const std::vector<Collision2DManifold>& Collision2DSystem::getNCollisionsManifolds() const
{
    return ncol_manifolds;
}
