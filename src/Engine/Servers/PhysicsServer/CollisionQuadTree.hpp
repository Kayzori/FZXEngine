#pragma once
#include <Math/Math.hpp>
#include <Engine/Object/2D/Collision2D.hpp>
#include <Engine/Renderer/2D/Renderer2D.hpp>
#include <unordered_set>

class CollisionQuadTree {
public:
    CollisionQuadTree(int level, const AABB bounds);
    ~CollisionQuadTree();

    static const int CAPACITY = 3;
    static const int DEPTH = 5;

    void render();
    void update();

    void clear();
    void insert(Collision2D* obj);
    bool remove(Collision2D* obj);
    void setBoard(const AABB newBoard);
    AABB getBoard() const;

    void retrieve(Collision2D* obj, std::vector<Collision2D*>& out);

private:
    int level;
    AABB board;

    std::unordered_set<Collision2D*> objects;       // per-node
    std::unordered_set<Collision2D*> allObjects;    // NEW: global registry

    CollisionQuadTree* subtrees[4];

    void tryMerge();
    void subdivide();
    int getIndex(Collision2D* obj) const;
};

