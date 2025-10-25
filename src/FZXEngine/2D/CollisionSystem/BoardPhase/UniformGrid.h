#pragma once

#include "../Collision2D.h"
#include "Math/Algebra/Vector/Vector2.h"
#include "Math/Geometry/AABB2D.h"

#include <unordered_map>
#include <vector>

//  UniformGrid
//  Time complexity per update: O(m) where m is number of occupied cells
//  Time complexity per pair computation: O(p) where p is number of potential pairs

struct CellKey {
    int x, y;

    bool operator==(const CellKey& other) const noexcept {
        return x == other.x && y == other.y;
    }
};

struct CellKeyHash {
    std::size_t operator()(const CellKey& key) const noexcept {
        return std::hash<int>()(key.x * 73856093 ^ key.y * 19349663);
    }
};

struct Cell {
    std::vector<Collision2D*> objects;
};

class UniformGrid {
public:
    UniformGrid(float cellSize = 50.0f);

    // n: number of objects
    // m: number of occupied cells
    // k: number of queried objects
    // p: number of potential pairs
                                                                                // Time Complexity
    void clear();                                                               // Best O(1), Average O(1), Worst O(1)
    void addObject(Collision2D* obj);                                           // Best O(1), Average O(1), Worst O(n)
    void build(const std::vector<Collision2D*>& objects);                       // Best O(m), Average O(m), Worst O(mn)
    std::vector<Collision2D*> queryNearby(const AABB2D& aabb) const;            // Best O(k), Average O(k), Worst O(kn)
    std::vector<std::pair<Collision2D*, Collision2D*>> computePairs() const;    // Best O(p), Average O(p), Worst O(pn)

    inline float getCellSize() const { return cell_size; }                      // Best O(1), Average O(1), Worst O(1)

private:
    float cell_size;
    std::unordered_map<CellKey, Cell, CellKeyHash> grid;

    inline CellKey getCellKey(const Vector2& pos) const;                        // Best O(1), Average O(1), Worst O(1)
};
