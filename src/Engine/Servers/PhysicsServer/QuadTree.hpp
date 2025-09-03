#pragma once
#include <Math/Math.hpp>
#include <Engine/Object/2D/Collision2D.hpp>

class QuadTree {
public:
    // Constructor
    QuadTree(int level, const AABB bounds);

    // CONSTS
    static const int CAPACITY = 12;
    static const int DEPTH = 8;

    // Methods
    void clear();
    void insert(Collision2D* obj);
    bool remove(Collision2D* obj);
    void retrieve(std::unordered_set<Collision2D*>& out, Collision2D* obj) const;
    void setBoard(const AABB newBoard);
    AABB getBoard();

private:
    // Properties
    int level;
    AABB board;
    std::unordered_set<Collision2D*> objects;
    std::unique_ptr<QuadTree> subtrees[4];

    // Methods
    void tryMerge();
    void subdivide();
    int getIndex(Collision2D* obj) const;
};
