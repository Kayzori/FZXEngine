#include "QuadTree.hpp"

// Constructor
QuadTree::QuadTree(int _level, const AABB _board)
    : level(_level), board(_board) {
    for (int i = 0; i < 4; ++i) subtrees[i].reset();
}

// Methods
void QuadTree::clear() {
    objects.clear();
    for (int i = 0; i < 4; ++i) {
        if (subtrees[i]) {
            subtrees[i]->clear();
            subtrees[i].reset();
        }
    }
}

void QuadTree::subdivide() {
    glm::vec2 hs = board.halfSize * 0.5f;
    glm::vec2 c = board.center;
    // TL, TR, BL, BR
    subtrees[0] = std::make_unique<QuadTree>(level + 1, AABB({c.x - hs.x, c.y - hs.y}, hs));
    subtrees[1] = std::make_unique<QuadTree>(level + 1, AABB({c.x + hs.x, c.y - hs.y}, hs));
    subtrees[2] = std::make_unique<QuadTree>(level + 1, AABB({c.x - hs.x, c.y + hs.y}, hs));
    subtrees[3] = std::make_unique<QuadTree>(level + 1, AABB({c.x + hs.x, c.y + hs.y}, hs));
}

int QuadTree::getIndex(Collision2D* obj) const {
    if (!subtrees[0]) return -1; // no children

    AABB query = AABB(obj->getVertices());
    for (int i = 0; i < 4; ++i) {
        if (subtrees[i]->board.contains(obj->getVertices())) return i;
    }
    return -1;
}

void QuadTree::insert(Collision2D* obj) {
    if (subtrees[0]) {
        int idx = getIndex(obj);
        if (idx != -1) {
            subtrees[idx]->insert(obj);
            return;
        }
    }

    objects.insert(obj);

    if ((int)objects.size() > CAPACITY && level < DEPTH) {
        if (!subtrees[0]) subdivide();

        for (auto it = objects.begin(); it != objects.end();) {
            int idx = getIndex(*it);
            if (idx != -1) {
                subtrees[idx]->insert(*it);
                it = objects.erase(it);
            } else {
                ++it;
            }
        }
    }
}

bool QuadTree::remove(Collision2D* obj) {
    if (!board.contains(obj->getVertices()))
        return false;

    if (subtrees[0]) {
        int idx = getIndex(obj);
        if (idx != -1) {
            bool removed = subtrees[idx]->remove(obj);
            if (removed) tryMerge();
            return removed;
        }
    }

    auto it = objects.find(obj);
    if (it != objects.end()) {
        objects.erase(it);
        return true;
    }

    return false;
}

void QuadTree::tryMerge() {
    if (!subtrees[0]) return;

    int totalObjects = (int)objects.size();
    for (int i = 0; i < 4; i++) {
        if (!subtrees[i]) return;
        totalObjects += (int)subtrees[i]->objects.size();
        if (subtrees[i]->subtrees[0]) return;
    }

    if (totalObjects <= CAPACITY) {
        for (int i = 0; i < 4; i++) {
            objects.insert(subtrees[i]->objects.begin(), subtrees[i]->objects.end());
            subtrees[i].reset();
        }
    }
}

void QuadTree::retrieve(std::unordered_set<Collision2D*>& out, Collision2D* obj) const {
    if (subtrees[0]) {
        AABB query = AABB(obj->getVertices());
        for (int i = 0; i < 4; ++i) {
            if (subtrees[i]->board.intersects(query)) {
                subtrees[i]->retrieve(out, obj);
            }
        }
    }
    out.insert(objects.begin(), objects.end()); // Merge sets
}

void QuadTree::setBoard(const AABB newBoard) {
    board = newBoard;
    clear(); // clear objects and children if resetting the board
}

AABB QuadTree::getBoard() {
    return board;
}
