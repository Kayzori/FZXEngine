#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <Engine/Object/2D/Collision2D.hpp>

// Axis-Aligned Bounding Box (center + half-size)
struct AABB {
    glm::vec2 center;
    glm::vec2 halfSize;

    AABB() : center(0.0f), halfSize(0.0f) {}
    AABB(const glm::vec2& c, const glm::vec2& hs) : center(c), halfSize(hs) {}

    bool contains(Collision2D* C) const;   // check if polygon fully inside this AABB
    bool intersects(const AABB& other) const;
};

class Quadtree {
public:
    static const int MAX_OBJECTS = 4;
    static const int MAX_LEVELS = 5;

    Quadtree(int level, const AABB& bounds);

    void clear();
    void insert(Collision2D* obj);
    void retrieve(std::vector<Collision2D*>& out, Collision2D* obj) const;

    void setBounds(const AABB& newBounds);
    AABB getBounds();

private:
    int level;
    AABB bounds;
    std::vector<Collision2D*> objects;
    std::unique_ptr<Quadtree> nodes[4];

    void split();
    int getIndex(Collision2D* obj) const;
};
