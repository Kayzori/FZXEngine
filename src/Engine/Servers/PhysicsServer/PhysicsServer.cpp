#include <Engine/Servers/PhysicsServer/PhysicsServer.hpp>
#include <algorithm>
#include <limits>
#include <glm/gtx/norm.hpp>

// ----------------- Init -----------------
void PhysicsServer::Init(const glm::vec2& center, const glm::vec2& halfSize) {
    tree.clear();
    Collision2DObjects.clear();
    tree.setBounds(AABB(center, halfSize));
}

// ----------------- Insert / Delete / Get -----------------
void PhysicsServer::InsertCollision(Collision2D* obj) {
    tree.insert(obj);
    Collision2DObjects.push_back(obj);
}

void PhysicsServer::DeleteCollision(Collision2D* obj) {
    auto it = std::find(Collision2DObjects.begin(), Collision2DObjects.end(), obj);
    if (it != Collision2DObjects.end()) Collision2DObjects.erase(it);

    tree.clear(); // rebuild tree for simplicity
    for (auto* o : Collision2DObjects) tree.insert(o);
}

const std::vector<Collision2D*>& PhysicsServer::GetCollisions() {
    return Collision2DObjects;
}

// ----------------- Narrow Phase: SAT -----------------
bool PhysicsServer::SATCollision2DDetection(Collision2D* A, Collision2D* B) {
    auto aVerts = A->getVertices();
    auto bVerts = B->getVertices();

    // Quick AABB check first
    auto computeAABB = [](const std::vector<glm::vec2>& verts) {
        glm::vec2 min(std::numeric_limits<float>::max());
        glm::vec2 max(std::numeric_limits<float>::lowest());
        for (const auto& v : verts) {
            min.x = std::min(min.x, v.x);
            min.y = std::min(min.y, v.y);
            max.x = std::max(max.x, v.x);
            max.y = std::max(max.y, v.y);
        }
        return std::make_pair(min, max);
    };

    auto [aMin, aMax] = computeAABB(aVerts);
    auto [bMin, bMax] = computeAABB(bVerts);

    if (aMax.x < bMin.x || bMax.x < aMin.x ||
        aMax.y < bMin.y || bMax.y < aMin.y) {
        return false;
    }

    auto project = [](const std::vector<glm::vec2>& poly, const glm::vec2& axis) {
        float minProj = std::numeric_limits<float>::max();
        float maxProj = std::numeric_limits<float>::lowest();
        for (const auto& v : poly) {
            float proj = glm::dot(v, axis);
            minProj = std::min(minProj, proj);
            maxProj = std::max(maxProj, proj);
        }
        return std::make_pair(minProj, maxProj);
    };

    auto checkAxes = [&](const std::vector<glm::vec2>& polyA, const std::vector<glm::vec2>& polyB) {
        for (size_t i = 0; i < polyA.size(); i++) {
            glm::vec2 edge = polyA[(i+1)%polyA.size()] - polyA[i];
            glm::vec2 axis = glm::normalize(glm::vec2(-edge.y, edge.x));

            auto [minA, maxA] = project(polyA, axis);
            auto [minB, maxB] = project(polyB, axis);

            if (maxA < minB || maxB < minA) return false;
        }
        return true;
    };

    return checkAxes(aVerts, bVerts) && checkAxes(bVerts, aVerts);
}

// ----------------- Broad + Narrow Phase -----------------
bool PhysicsServer::Collision2DDetection(Collision2D* obj) {
    std::vector<Collision2D*> candidates;
    tree.retrieve(candidates, obj); // broad-phase

    for (auto* other : candidates) {
        if (other == obj) continue;
        if (SATCollision2DDetection(obj, other)) return true;
    }
    return false;
}
