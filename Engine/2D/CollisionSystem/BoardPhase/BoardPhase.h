#pragma once

#include "../Collision2D.h"

#ifdef FZX_2D_BOARD_PHASE_USE_UNIFORM_GRID
#include "UniformGrid.h"
#endif

class BoardPhase
{
private:
    std::vector<Collision2D*> objects;
    #ifdef FZX_2D_BOARD_PHASE_USE_UNIFORM_GRID
        UniformGrid* grid;
    #endif
public:
    // Constructors and Destructors
    #ifdef FZX_2D_BOARD_PHASE_USE_UNIFORM_GRID
    BoardPhase(float cellSize = 50.0f) : grid(new UniformGrid(cellSize)) {}
    ~BoardPhase() {delete grid;}
    #endif

    // Methods
    void addObject(Collision2D* obj) {
        objects.push_back(obj);
    }

    void removeObject(Collision2D* obj) {
        objects.erase(std::remove(objects.begin(), objects.end(), obj), objects.end());
    }

    std::vector<Collision2D*> query(const AABB2D& aabb) const {
        #ifdef FZX_2D_BOARD_PHASE_USE_UNIFORM_GRID
        return grid->queryNearby(aabb);
        #else
        std::vector<Collision2D*> result;
        for (Collision2D* obj : objects) {
            if (obj->shape) {
                const AABB2D objAABB = AABB2D(obj->shape->getVertices());
                if (aabb.intersects(objAABB)) {
                    result.push_back(obj);
                }
            }
        }
        return result;
        #endif
    }

    std::vector<std::pair<Collision2D*, Collision2D*>> computePairs() const {
        #ifdef FZX_2D_BOARD_PHASE_USE_UNIFORM_GRID
        // p: 
        // n: number of objects
        // Best O(p), Average O(p), Worst O(pn)
        return grid->computePairs();
        #else
        // Naive O(n^2) broad-phase collision detection
        std::vector<std::pair<Collision2D*, Collision2D*>> pairs;
        for (size_t i = 0; i < objects.size(); ++i) {
            for (size_t j = i + 1; j < objects.size(); ++j) {
                if (objects[i]->shape && objects[j]->shape) {
                    // AABB collision detection for optimal broad-phase
                    const AABB2D aabb1 = AABB2D(objects[i]->shape->getVertices());
                    const AABB2D aabb2 = AABB2D(objects[j]->shape->getVertices());
                    if (aabb1.intersects(aabb2)) {
                        pairs.emplace_back(objects[i], objects[j]);
                    }
                }
            }
        }
        return pairs;
        #endif
    }

    void update() {
        #ifdef FZX_2D_BOARD_PHASE_USE_UNIFORM_GRID
        // m: number of occupied cells
        // n: number of objects
        // Best O(m), Average O(m), Worst O(mn) (per update)
        grid->build(objects);
        #endif
    }
};
