#pragma once

#include "Collision2D.h"

#ifdef FZX_2D_COLLISION_SYSTEM_USE_UNIFORM_GRID
#include "UniformGrid.h"
#endif

#include "Math/Geometry2D/Core/AABB2D.h"
#include <iostream>

class Collision2DSystem : public Node2D
{
private:
    // Board-phase properties
    std::vector<Collision2D*> objects;
#ifdef FZX_2D_COLLISION_SYSTEM_USE_UNIFORM_GRID
    UniformGrid* grid;
#endif
    // Narrow-phase properties
    std::vector<Collision2DManifold> manifolds;
    std::vector<Collision2DManifold> col_manifolds;
    std::vector<Collision2DManifold> ncol_manifolds;

public:
    // Constructors and Destructors
    #ifdef FZX_2D_COLLISION_SYSTEM_USE_UNIFORM_GRID
    Collision2DSystem(float cellSize = 50.0f) : grid(new UniformGrid(cellSize)) {}
    ~Collision2DSystem() {delete grid;}
    #endif

    // Board-Phase methods
    void addObject(Collision2D* obj);

    void removeObject(Collision2D* obj);

    std::vector<Collision2D*> query(const AABB2D& aabb) const;

    std::vector<std::pair<Collision2D*, Collision2D*>> computePairs() const;

    // Narrow-Phase methods
    void updateManifolds();

    const std::vector<Collision2DManifold>& getManifolds() const;
    const std::vector<Collision2DManifold>& getCollisionsManifolds() const;
    const std::vector<Collision2DManifold>& getNCollisionsManifolds() const;

    // Game Loop
    void onUpdate(float dt) override {
        #ifdef FZX_2D_COLLISION_SYSTEM_USE_UNIFORM_GRID
        // m: number of occupied cells
        // n: number of objects
        // Best O(m), Average O(m), Worst O(mn) (per update)
        grid->build(objects);
        #endif

        // Debug output: show counts for debug builds
        size_t objCount = objects.size();
        size_t pairCount = computePairs().size();

        updateManifolds();

        size_t colCount = col_manifolds.size();
        size_t ncolCount = ncol_manifolds.size();

        // Print a concise debug summary
        std::cout << "[Collision2DSystem] update dt=" << dt
                  << " objs=" << objCount
                  << " pairs=" << pairCount
                  << " collisions=" << colCount
                  << " non-collisions=" << ncolCount << std::endl;
    }

    void onRender(Renderer* renderer) override {
        size_t colCount = col_manifolds.size();
        std::vector<Vector2> contacts;
        for (Collision2DManifold& man : col_manifolds) {
            for (ContactPoint2D& c : man.contacts) {
                contacts.push_back(c.position);
            }
        }
        // draw contact points slightly larger so they're visible
        renderer->drawPolygonVertices(contacts, Vector4(1.0f, 0.0f, 0.0f, 1.0f), 10.0f);
    }
};
