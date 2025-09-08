#pragma once
#include <Math/Math.hpp>
#include <Engine/Object/2D/Object2D.h>

class CollisionSpatialGrid {
public:
    int CellCount = 100;
    AABB Board;
    std::vector<Collision2D*> Objects;
    std::unordered_map<uint64_t, std::vector<Collision2D*>> Grid;

    CollisionSpatialGrid(const AABB& board = AABB()) : Board(board) {}
    
    void Update();
    void Render();

    std::vector<std::pair<Collision2D*, Collision2D*>> CollectPhyisicsPair();
    
    void Clear() {
        Objects.clear();
        Grid.clear();
    }
    
    void AddObject(Collision2D* obj) {
        Objects.push_back(obj);
    }
    
    void RemoveObject(Collision2D* obj) {
        Objects.erase(std::remove(Objects.begin(), Objects.end(), obj), Objects.end());
    }
};