#include <Engine/Servers/PhysicsServer/QuadTree.hpp>
#include <limits>
#include <algorithm>

// ---------------------- AABB ----------------------
bool AABB::contains(Collision2D* C) const {
    auto verts = C->getVertices();
    for (auto& v : verts) {
        if (v.x < center.x - halfSize.x || v.x > center.x + halfSize.x ||
            v.y < center.y - halfSize.y || v.y > center.y + halfSize.y)
            return false;
    }
    return true;
}

bool AABB::intersects(const AABB& other) const {
    return !(other.center.x - other.halfSize.x > center.x + halfSize.x ||
             other.center.x + other.halfSize.x < center.x - halfSize.x ||
             other.center.y - other.halfSize.y > center.y + halfSize.y ||
             other.center.y + other.halfSize.y < center.y - halfSize.y);
}

// -------------------- Quadtree --------------------
Quadtree::Quadtree(int _level, const AABB& _bounds)
    : level(_level), bounds(_bounds) {
    for (int i = 0; i < 4; ++i) nodes[i].reset();
}

void Quadtree::clear() {
    objects.clear();
    for (int i = 0; i < 4; ++i) {
        if (nodes[i]) {
            nodes[i]->clear();
            nodes[i].reset();
        }
    }
}

void Quadtree::split() {
    glm::vec2 hs = bounds.halfSize * 0.5f;
    glm::vec2 c = bounds.center;
    // TL, TR, BL, BR
    nodes[0] = std::make_unique<Quadtree>(level + 1, AABB({c.x - hs.x, c.y - hs.y}, hs));
    nodes[1] = std::make_unique<Quadtree>(level + 1, AABB({c.x + hs.x, c.y - hs.y}, hs));
    nodes[2] = std::make_unique<Quadtree>(level + 1, AABB({c.x - hs.x, c.y + hs.y}, hs));
    nodes[3] = std::make_unique<Quadtree>(level + 1, AABB({c.x + hs.x, c.y + hs.y}, hs));
}

// compute AABB from polygon vertices
static AABB computeAABBFromVerts(Collision2D* obj) {
    auto verts = obj->getVertices();
    glm::vec2 mn(std::numeric_limits<float>::max());
    glm::vec2 mx(std::numeric_limits<float>::lowest());
    for (auto& v : verts) {
        mn.x = std::min(mn.x, v.x);
        mn.y = std::min(mn.y, v.y);
        mx.x = std::max(mx.x, v.x);
        mx.y = std::max(mx.y, v.y);
    }
    glm::vec2 center = (mn + mx) * 0.5f;
    glm::vec2 half = (mx - mn) * 0.5f;
    return AABB(center, half);
}

int Quadtree::getIndex(Collision2D* obj) const {
    if (!nodes[0]) return -1; // no children

    AABB aabb = computeAABBFromVerts(obj);
    for (int i = 0; i < 4; ++i) {
        if (nodes[i]->bounds.contains(obj)) return i;
    }
    return -1;
}

void Quadtree::insert(Collision2D* obj) {
    if (nodes[0]) {
        int idx = getIndex(obj);
        if (idx != -1) {
            nodes[idx]->insert(obj);
            return;
        }
    }

    objects.push_back(obj);

    if ((int)objects.size() > MAX_OBJECTS && level < MAX_LEVELS) {
        if (!nodes[0]) split();

        auto it = objects.begin();
        while (it != objects.end()) {
            int idx = getIndex(*it);
            if (idx != -1) {
                nodes[idx]->insert(*it);
                it = objects.erase(it);
            } else ++it;
        }
    }
}

void Quadtree::retrieve(std::vector<Collision2D*>& out, Collision2D* obj) const {
    if (nodes[0]) {
        AABB query = computeAABBFromVerts(obj);
        for (int i = 0; i < 4; ++i) {
            if (nodes[i]->bounds.intersects(query)) {
                nodes[i]->retrieve(out, obj);
            }
        }
    }
    out.insert(out.end(), objects.begin(), objects.end());
}

void Quadtree::setBounds(const AABB& newBounds) {
    bounds = newBounds;
    clear(); // optional: clear objects and children if resetting the bounds
}

AABB Quadtree::getBounds() {
    return bounds;
}
