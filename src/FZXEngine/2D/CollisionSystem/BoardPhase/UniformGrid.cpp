#include "UniformGrid.h"

// Constructor

UniformGrid::UniformGrid(float cellSize)
    : cell_size(cellSize) {}

// Methods

void UniformGrid::clear() {
    grid.clear();
}

inline CellKey UniformGrid::getCellKey(const Vector2& pos) const {
    return {
        static_cast<int>(std::floor(pos.x / cell_size)),
        static_cast<int>(std::floor(pos.y / cell_size))
    };
}

void UniformGrid::addObject(Collision2D* obj) {
    if (!obj || !obj->shape) return;

    const AABB2D aabb = AABB2D(obj->shape->getVertices());

    int min_x = static_cast<int>(std::floor(aabb.min().x / cell_size));
    int min_y = static_cast<int>(std::floor(aabb.min().y / cell_size));
    int max_x = static_cast<int>(std::floor(aabb.max().x / cell_size));
    int max_y = static_cast<int>(std::floor(aabb.max().y / cell_size));

    for (int y = min_y; y <= max_y; ++y) {
        for (int x = min_x; x <= max_x; ++x) {
            grid[{x, y}].objects.push_back(obj);
        }
    }
}

void UniformGrid::build(const std::vector<Collision2D*>& objects) {
    grid.clear();
    grid.reserve(objects.size() * 2); // reduce rehashing

    for (Collision2D* obj : objects) {
        addObject(obj);
    }
}

std::vector<Collision2D*> UniformGrid::queryNearby(const AABB2D& aabb) const {
    std::vector<Collision2D*> result;
    result.reserve(32);

    int min_x = static_cast<int>(std::floor(aabb.min().x / cell_size));
    int min_y = static_cast<int>(std::floor(aabb.min().y / cell_size));
    int max_x = static_cast<int>(std::floor(aabb.max().x / cell_size));
    int max_y = static_cast<int>(std::floor(aabb.max().y / cell_size));

    for (int y = min_y; y <= max_y; ++y) {
        for (int x = min_x; x <= max_x; ++x) {
            auto it = grid.find({x, y});
            if (it != grid.end()) {
                const auto& cellObjs = it->second.objects;
                result.insert(result.end(), cellObjs.begin(), cellObjs.end());
            }
        }
    }

    return result;
}

std::vector<std::pair<Collision2D*, Collision2D*>> UniformGrid::computePairs() const {
    std::vector<std::pair<Collision2D*, Collision2D*>> pairs;
    pairs.reserve(grid.size() * 4);

    for (const auto& [key, cell] : grid) {
        const auto& objs = cell.objects;
        for (size_t i = 0; i < objs.size(); ++i) {
            for (size_t j = i + 1; j < objs.size(); ++j) {
                // Collect potential collision pairs
                if (AABB2D(objs[i]->shape->getVertices()).intersects(AABB2D(objs[j]->shape->getVertices())))
                    pairs.emplace_back(objs[i], objs[j]);
            }
        }
    }

    return pairs;
}
