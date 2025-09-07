#include "CollisionQuadTree.hpp"

// QuadTree --------------------------------------------------------------------

CollisionQuadTree::CollisionQuadTree(int _level, const AABB _board)
    : level(_level), board(_board) {
    for (int i = 0; i < 4; ++i) subtrees[i] = nullptr;
}

CollisionQuadTree::~CollisionQuadTree() {
    clear();
}

void CollisionQuadTree::render() {
    if (subtrees[0]) {
        Renderer2D::DrawLines({{board.x, board.y + board.hh}, {board.x, board.y - board.hh}}, {1, 1, 1, 1});
        Renderer2D::DrawLines({{board.x + board.hw, board.y}, {board.x - board.hw, board.y}}, {1, 1, 1, 1});
        for (int i = 0; i < 4; i++) {
            if (subtrees[i]) subtrees[i]->render();
        }
    }
}

void CollisionQuadTree::update() {
    std::unordered_set<Collision2D*> snapshot = allObjects;
    objects.clear();
    for (int i = 0; i < 4; ++i) {
        if (subtrees[i]) {
            delete subtrees[i];
            subtrees[i] = nullptr;
        }
    }
    for (Collision2D* obj : snapshot) {
        insert(obj);
    }
}

void CollisionQuadTree::clear() {
    objects.clear();
    for (int i = 0; i < 4; ++i) {
        if (subtrees[i]) {
            subtrees[i]->clear();
            delete subtrees[i];
            subtrees[i] = nullptr;
        }
    }
}

void CollisionQuadTree::subdivide() {
    glm::vec2 hs = board.halfSize * 0.5f;
    glm::vec2 c = board.center;
    subtrees[0] = new CollisionQuadTree(level + 1, AABB({c.x - hs.x, c.y - hs.y}, hs));
    subtrees[1] = new CollisionQuadTree(level + 1, AABB({c.x + hs.x, c.y - hs.y}, hs));
    subtrees[2] = new CollisionQuadTree(level + 1, AABB({c.x - hs.x, c.y + hs.y}, hs));
    subtrees[3] = new CollisionQuadTree(level + 1, AABB({c.x + hs.x, c.y + hs.y}, hs));
}

int CollisionQuadTree::getIndex(Collision2D* obj) const {
    if (!subtrees[0]) return -1;
    for (int i = 0; i < 4; ++i) {
        if (subtrees[i]->board.contains(obj->getVertices())) {
            return i;
        }
    }
    return -1;
}

void CollisionQuadTree::insert(Collision2D* obj) {
    AABB objBounds = obj->getBounds();
    if (!board.intersects(objBounds)) return;
    if (subtrees[0]) {
        int idx = getIndex(obj);
        if (idx != -1) {
            subtrees[idx]->insert(obj);
            return;
        }
    }
    objects.insert(obj);
    allObjects.insert(obj);
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

bool CollisionQuadTree::remove(Collision2D* obj) {
    AABB objBounds = obj->getBounds();
    if (!board.intersects(objBounds)) return false;
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
        allObjects.erase(obj);
        tryMerge();
        return true;
    }
    return false;
}

void CollisionQuadTree::tryMerge() {
    if (!subtrees[0]) return;
    int totalObjects = objects.size();
    bool anyChildHasChildren = false;
    bool anyChildHasObjects = false;
    for (int i = 0; i < 4; i++) {
        if (!subtrees[i]) continue;
        totalObjects += subtrees[i]->objects.size();
        if (subtrees[i]->subtrees[0]) anyChildHasChildren = true;
        if (!subtrees[i]->objects.empty()) anyChildHasObjects = true;
    }
    if (!anyChildHasChildren && totalObjects <= CAPACITY) {
        for (int i = 0; i < 4; i++) {
            if (subtrees[i]) {
                objects.insert(subtrees[i]->objects.begin(), subtrees[i]->objects.end());
                delete subtrees[i];
                subtrees[i] = nullptr;
            }
        }
    }
}

void CollisionQuadTree::setBoard(const AABB newBoard) {
    board = newBoard;
    clear();
}

AABB CollisionQuadTree::getBoard() const {
    return board;
}

void CollisionQuadTree::retrieve(Collision2D* obj, std::vector<Collision2D*>& out) {
    if (!board.intersects(obj->getVertices())) return;
    AABB objBounds = obj->getBounds();
    for (Collision2D* other : objects) {
        if (other != obj && objBounds.intersects(other->getBounds())) {
            out.push_back(other);
        }
    }
    if (subtrees[0]) {
        for (int i = 0; i < 4; ++i) {
            if (subtrees[i]->board.intersects(obj->getBounds())) {
                subtrees[i]->retrieve(obj, out);
            }
        }
    }
}
