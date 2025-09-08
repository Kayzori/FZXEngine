#include "CollisionSpatialGrid.hpp"
#include <Engine/Renderer/2D/Renderer2D.hpp>
#include <algorithm>
#include <functional>

void CollisionSpatialGrid::Update() {
    Grid.clear();
    const float cellSize = Board.hw * 2.0f / float(CellCount);
    
    for (Collision2D* obj : Objects) {
        const AABB& aabb = obj->getBounds();
        int minX = static_cast<int>((aabb.x - aabb.hw - Board.x + Board.hw) / cellSize);
        int maxX = static_cast<int>((aabb.x + aabb.hw - Board.x + Board.hw) / cellSize);
        int minY = static_cast<int>((aabb.y - aabb.hh - Board.y + Board.hh) / cellSize);
        int maxY = static_cast<int>((aabb.y + aabb.hh - Board.y + Board.hh) / cellSize);
        
        for (int x = minX; x <= maxX; ++x) {
            for (int y = minY; y <= maxY; ++y) {
                uint64_t cellKey = (static_cast<uint64_t>(x) << 32) | static_cast<uint32_t>(y);
                Grid[cellKey].push_back(obj);
            }
        }
    }
}

void CollisionSpatialGrid::Render() {
    const float cellSize = Board.hw * 2.0f / float(CellCount);
    for (int i = 0; i <= CellCount; i++) {
        float x = Board.x - Board.hw + i * cellSize;
        float y = Board.y - Board.hh + i * cellSize;

        Renderer2D::DrawLines({{x, Board.y - Board.hh}, {x, Board.y + Board.hh}}, {1, 1, 1, 1});
        Renderer2D::DrawLines({{Board.x - Board.hw, y}, {Board.x + Board.hw, y}}, {1, 1, 1, 1});
    }
}


struct PairHash {
    size_t operator()(const std::pair<Collision2D*, Collision2D*>& p) const {
        // Always hash in canonical order
        Collision2D* first = std::min(p.first, p.second);
        Collision2D* second = std::max(p.first, p.second);
        return std::hash<Collision2D*>()(first) ^ (std::hash<Collision2D*>()(second) << 1);
    }
};

struct PairEqual {
    bool operator()(const std::pair<Collision2D*, Collision2D*>& a, const std::pair<Collision2D*, Collision2D*>& b) const {
        auto canonA = std::make_pair(std::min(a.first, a.second), std::max(a.first, a.second));
        auto canonB = std::make_pair(std::min(b.first, b.second), std::max(b.first, b.second));
        return canonA == canonB;
    }
};

std::vector<std::pair<Collision2D*, Collision2D*>> CollisionSpatialGrid::CollectPhyisicsPair() {
    std::unordered_set<std::pair<Collision2D*, Collision2D*>, PairHash, PairEqual> uniquePairs;
    
    for (auto& cell : Grid) {
        auto& objectsInCell = cell.second;
        
        for (size_t i = 0; i < objectsInCell.size(); ++i) {
            Collision2D* a = objectsInCell[i];
            if(dynamic_cast<RigidBody2D*>(a->PHYSICS_PARENT))
                for (size_t j = i + 1; j < objectsInCell.size(); ++j) {
                    if (i == j) continue;
                    Collision2D* b = objectsInCell[j];
                    if(!dynamic_cast<PhysicsBody2D*>(b->PHYSICS_PARENT)) continue;
                    if (a == b) continue;
                    
                    if (a->getBounds().intersects(b->getBounds())) {
                        uniquePairs.emplace(a, b);
                    }
                }
            else if(dynamic_cast<PhysicsBody2D*>(a->PHYSICS_PARENT))
                for (size_t j = i + 1; j < objectsInCell.size(); ++j) {
                    if (i == j) continue;
                    Collision2D* b = objectsInCell[j];
                    if(!dynamic_cast<RigidBody2D*>(b->PHYSICS_PARENT)) continue;
                    if (a == b) continue;
                    
                    if (a->getBounds().intersects(b->getBounds())) {
                        uniquePairs.emplace(b, a);
                    }
                }
        }
    }
    
    return std::vector<std::pair<Collision2D*, Collision2D*>>(uniquePairs.begin(), uniquePairs.end());
}


