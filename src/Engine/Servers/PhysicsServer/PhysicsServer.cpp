#include "PhysicsServer.hpp"

// ------------------------- Init --------------------------
void Physics2DServer::CollisionSystem::InitCollisionBoard(AABB board) {
    tree.clear();
    Collision2DObjects.clear();
    tree.setBoard(board);
}

// ----------------- Insert / Delete / Get -----------------
void Physics2DServer::CollisionSystem::InsertCollision(Collision2D* obj) {
    tree.insert(obj);
    Collision2DObjects.insert(obj);
}

void Physics2DServer::CollisionSystem::DeleteCollision(Collision2D* obj) {
    Collision2DObjects.erase(obj);
    tree.remove(obj);
}

const std::unordered_set<Collision2D*>& Physics2DServer::CollisionSystem::GetCollisions() {
    return Collision2DObjects;
}

// ---------- Base Collision Detection Algorithm -----------
Collision2DInfos Physics2DServer::CollisionSystem::CollisionDetection(Collision2D* A, Collision2D* B) {
    Collision2DInfos info = Collision2DInfos();

    // 1. Get world-space vertices
    const std::vector<glm::vec2>& vertsA = A->getVertices();
    const std::vector<glm::vec2>& vertsB = B->getVertices();

    if (vertsA.empty() || vertsB.empty()) return info;

    // 2. Early AABB check
    AABB aabbA(A->getVertices());
    AABB aabbB(B->getVertices());
    
    if (!aabbA.intersects(aabbB)) {
        return info;
    }

    // 3. Circle vs Circle check
    Circle2D* circleA = dynamic_cast<Circle2D*>(A->shape);
    Circle2D* circleB = dynamic_cast<Circle2D*>(B->shape);
    if (circleA && circleB) {
        glm::vec2 centerA = A->transform->position;
        glm::vec2 centerB = B->transform->position;
        
        glm::vec2 delta = centerB - centerA;
        float distSq = glm::dot(delta, delta);
        float totalRadius = circleA->radius + circleB->radius;
        float totalRadiusSq = totalRadius * totalRadius;

        if (distSq <= totalRadiusSq) {
            info.isColliding = true;
            if (A->PHYSICS_PARENT && B->PHYSICS_PARENT) {
                info.isPhysicsColliding = true;
                float dist = std::sqrt(distSq);
                glm::vec2 normal = (dist > 0.0001f) ? delta / dist : glm::vec2(1.0f, 0.0f);
                float penetration = totalRadius - dist;
                
                // MTV should point from A to B (separation direction)
                info.MTV = -1.0f * normal * penetration;
                
                // Contact point is the midpoint between the two circle surfaces
                glm::vec2 contact = centerA + normal * (circleA->radius - penetration * 0.5f);
                info.ContactPoints.push_back(contact - A->transform->position);
            }
        }
        return info;
    }

    // 4. Circle vs Polygon check
    Circle2D* circle = nullptr;
    const std::vector<glm::vec2>* polyVerts = nullptr;
    Collision2D* circleObj = nullptr;
    Collision2D* polyObj = nullptr;
    glm::vec2 circleCenter;
    int sign;

    if ((circle = dynamic_cast<Circle2D*>(A->shape)) && vertsB.size() >= 3) {
        sign = 1;
        circleObj = A;
        polyObj = B;
        polyVerts = &vertsB;
        circleCenter = A->transform->position;
    } 
    else if ((circle = dynamic_cast<Circle2D*>(B->shape)) && vertsA.size() >= 3) {
        sign = -1;
        circleObj = B;
        polyObj = A;
        polyVerts = &vertsA;
        circleCenter = B->transform->position;
    }

    if (circle && polyVerts) {
        float minDist = std::numeric_limits<float>::max();
        glm::vec2 closestPoint;
        glm::vec2 normal;

        auto closestPointOnSegment = [&](const glm::vec2 point, const glm::vec2 a, const glm::vec2 b) {
            glm::vec2 ab = b - a;
            float t = glm::dot(point - a, ab) / glm::dot(ab, ab);
            t = std::clamp(t, 0.0f, 1.0f);
            return a + t * ab;
        };

        // Check against polygon edges
        for (size_t i = 0; i < polyVerts->size(); i++) {
            size_t j = (i + 1) % polyVerts->size();
            glm::vec2 edgeStart = (*polyVerts)[i];
            glm::vec2 edgeEnd = (*polyVerts)[j];
            
            glm::vec2 pointOnEdge = closestPointOnSegment(circleCenter, edgeStart, edgeEnd);
            float dist = glm::distance(circleCenter, pointOnEdge);
            
            if (dist < minDist) {
                minDist = dist;
                closestPoint = pointOnEdge;
                normal = glm::normalize(circleCenter - pointOnEdge);
            }
        }

        // Also check if circle center is inside polygon
        auto isPointInPolygon = [&](const glm::vec2& point, const std::vector<glm::vec2>& poly) {
            int winding = 0;
            for (size_t i = 0; i < poly.size(); i++) {
                size_t j = (i + 1) % poly.size();
                if (poly[i].y <= point.y) {
                    if (poly[j].y > point.y && 
                        (poly[j].x - poly[i].x) * (point.y - poly[i].y) > (point.x - poly[i].x) * (poly[j].y - poly[i].y)) {
                        winding++;
                    }
                } else {
                    if (poly[j].y <= point.y && 
                        (poly[j].x - poly[i].x) * (point.y - poly[i].y) < (point.x - poly[i].x) * (poly[j].y - poly[i].y)) {
                        winding--;
                    }
                }
            }
            return winding != 0;
        };

        bool centerInside = isPointInPolygon(circleCenter, *polyVerts);
        
        if (centerInside || minDist <= circle->radius) {
            info.isColliding = true;
            if (A->PHYSICS_PARENT && B->PHYSICS_PARENT) {
                info.isPhysicsColliding = true;
                
                if (centerInside) {
                    // Find closest polygon edge when circle is inside
                    minDist = std::numeric_limits<float>::max();
                    for (size_t i = 0; i < polyVerts->size(); i++) {
                        size_t j = (i + 1) % polyVerts->size();
                        glm::vec2 edgeStart = (*polyVerts)[i];
                        glm::vec2 edgeEnd = (*polyVerts)[j];
                        
                        glm::vec2 edgeNormal = glm::normalize(glm::vec2(-(edgeEnd.y - edgeStart.y), edgeEnd.x - edgeStart.x));
                        float distToEdge = glm::dot(circleCenter - edgeStart, edgeNormal);
                        
                        if (distToEdge < minDist) {
                            minDist = distToEdge;
                            normal = edgeNormal;
                            closestPoint = circleCenter - edgeNormal * distToEdge;
                        }
                    }
                    float penetration = circle->radius + minDist;
                    info.MTV =  -1.0f * float(sign) * normal * penetration ;
                } else {
                    float penetration = circle->radius - minDist;
                    info.MTV = float(sign) * normal * penetration;
                }

                // Contact point is on the circle surface
                glm::vec2 contact = circleCenter - normal * circle->radius;
                info.ContactPoints.push_back(contact - circleObj->transform->position);
            }
        }
        return info;
    }

    // 5. General Polygon vs Polygon SAT
    float minOverlap = std::numeric_limits<float>::max();
    glm::vec2 mtvAxis;
    bool isFirstPolygonAxis = true;

    auto project = [](const std::vector<glm::vec2>& verts, const glm::vec2& axis) -> std::pair<float, float> {
        float minVal = glm::dot(axis, verts[0]);
        float maxVal = minVal;
        for (size_t i = 1; i < verts.size(); i++) {
            float proj = glm::dot(axis, verts[i]);
            minVal = std::min(minVal, proj);
            maxVal = std::max(maxVal, proj);
        }
        return {minVal, maxVal};
    };

    // Check axes from A
    for (size_t i = 0; i < vertsA.size(); i++) {
        glm::vec2 v1 = vertsA[i];
        glm::vec2 v2 = vertsA[(i + 1) % vertsA.size()];
        glm::vec2 edge = v2 - v1;
        glm::vec2 axis = glm::normalize(glm::vec2(-edge.y, edge.x));

        auto [minA, maxA] = project(vertsA, axis);
        auto [minB, maxB] = project(vertsB, axis);

        if (maxA < minB || maxB < minA) {
            return info;
        }

        float overlap = std::min(maxA, maxB) - std::max(minA, minB);
        if (overlap < minOverlap) {
            minOverlap = overlap;
            mtvAxis = axis;
            isFirstPolygonAxis = true;
        }
    }

    // Check axes from B
    for (size_t i = 0; i < vertsB.size(); i++) {
        glm::vec2 v1 = vertsB[i];
        glm::vec2 v2 = vertsB[(i + 1) % vertsB.size()];
        glm::vec2 edge = v2 - v1;
        glm::vec2 axis = glm::normalize(glm::vec2(-edge.y, edge.x));

        auto [minA, maxA] = project(vertsA, axis);
        auto [minB, maxB] = project(vertsB, axis);

        if (maxA < minB || maxB < minA) {
            return info;
        }

        float overlap = std::min(maxA, maxB) - std::max(minA, minB);
        if (overlap < minOverlap) {
            minOverlap = overlap;
            mtvAxis = axis;
            isFirstPolygonAxis = false;
        }
    }

    // Collision confirmed
    info.isColliding = true;
    
    if (A->PHYSICS_PARENT && B->PHYSICS_PARENT) {
        info.isPhysicsColliding = true;

        // Calculate centers to determine MTV direction
        auto calculateCenter = [](const std::vector<glm::vec2>& vertices) {
            glm::vec2 center(0.0f);
            for (const auto& vert : vertices) {
                center += vert;
            }
            return center / static_cast<float>(vertices.size());
        };

        glm::vec2 centerA = calculateCenter(vertsA);
        glm::vec2 centerB = calculateCenter(vertsB);
        glm::vec2 centerDir = centerB - centerA;

        // Ensure MTV points from A to B (separation direction)
        if (glm::dot(mtvAxis, centerDir) < 0) {
            mtvAxis = -mtvAxis;
        }

        info.MTV = -1.0f * mtvAxis * minOverlap;

        // Find contact points using clipping method
        auto findContactPoints = [&](const std::vector<glm::vec2>& poly1, const std::vector<glm::vec2>& poly2) {
            std::vector<glm::vec2> contacts;
            
            // Find reference and incident edges
            size_t refIndex = 0;
            float maxDot = -std::numeric_limits<float>::max();
            
            for (size_t i = 0; i < poly1.size(); i++) {
                float dot = glm::dot(mtvAxis, poly1[i] - centerA);
                if (dot > maxDot) {
                    maxDot = dot;
                    refIndex = i;
                }
            }
            
            glm::vec2 refV1 = poly1[refIndex];
            glm::vec2 refV2 = poly1[(refIndex + 1) % poly1.size()];
            glm::vec2 refEdge = refV2 - refV1;
            glm::vec2 refNormal = glm::normalize(glm::vec2(-refEdge.y, refEdge.x));
            
            // Find incident edge from poly2
            size_t incIndex = 0;
            float minDot = std::numeric_limits<float>::max();
            
            for (size_t i = 0; i < poly1.size(); i++) {
                glm::vec2 v1 = poly1[i];
                glm::vec2 v2 = poly1[(i+1) % poly1.size()];
                glm::vec2 edge = v2 - v1;
                glm::vec2 normal = glm::normalize(glm::vec2(-edge.y, edge.x));

                float dot = glm::dot(normal, mtvAxis);
                if (dot < minDot) {
                    minDot = dot;
                    refIndex = i;
                }
            }
            
            glm::vec2 incV1 = poly2[incIndex];
            glm::vec2 incV2 = poly2[(incIndex + 1) % poly2.size()];
            
            // Clip incident edge against reference edge
            auto clip = [](const glm::vec2& v1, const glm::vec2& v2, const glm::vec2& normal, float offset) {
                std::vector<glm::vec2> clipped;
                
                float d1 = glm::dot(normal, v1) - offset;
                float d2 = glm::dot(normal, v2) - offset;
                
                if (d1 >= 0) clipped.push_back(v1);
                if (d2 >= 0) clipped.push_back(v2);
                
                if (d1 * d2 < 0) {
                    float t = d1 / (d1 - d2);
                    clipped.push_back(v1 + t * (v2 - v1));
                }
                
                return clipped;
            };
            
            std::vector<glm::vec2> clipped = {incV1, incV2};
            clipped = clip(clipped[0], clipped[1], refNormal, glm::dot(refNormal, refV1));
            if (clipped.size() < 2) return contacts;
            
            clipped = clip(clipped[0], clipped[1], -refNormal, -glm::dot(refNormal, refV2));
            if (clipped.size() < 2) return contacts;
            
            // Keep points behind reference edge
            for (const auto& point : clipped) {
                if (glm::dot(point - refV1, refNormal) <= 0) {
                    contacts.push_back(point);
                }
            }
            
            return contacts;
        };

        // Get contact points from both perspectives and take the ones with deeper penetration
        std::vector<glm::vec2> contactsA = findContactPoints(vertsA, vertsB);
        std::vector<glm::vec2> contactsB = findContactPoints(vertsB, vertsA);
        
        if (!contactsA.empty()) {
            info.ContactPoints = contactsA;
        } else if (!contactsB.empty()) {
            info.ContactPoints = contactsB;
        }
        
        // Fallback: use vertex inclusion if clipping failed
        if (info.ContactPoints.empty()) {
            auto isInside = [](const std::vector<glm::vec2>& poly, const glm::vec2& point) {
                int winding = 0;
                for (size_t i = 0; i < poly.size(); i++) {
                    size_t j = (i + 1) % poly.size();
                    if (poly[i].y <= point.y) {
                        if (poly[j].y > point.y && 
                            (poly[j].x - poly[i].x) * (point.y - poly[i].y) > (point.x - poly[i].x) * (poly[j].y - poly[i].y)) {
                            winding++;
                        }
                    } else {
                        if (poly[j].y <= point.y && 
                            (poly[j].x - poly[i].x) * (point.y - poly[i].y) < (point.x - poly[i].x) * (poly[j].y - poly[i].y)) {
                            winding--;
                        }
                    }
                }
                return winding != 0;
            };

            for (const auto& vert : vertsA) {
                if (isInside(vertsB, vert)) {
                    info.ContactPoints.push_back(vert - A->transform->position);
                }
            }
        }
    }
    return info;
}

// ---------------- Collision Updates Infos ----------------
void Physics2DServer::CollisionSystem::UpdateCollisionInfos(Collision2D* obj) {
    std::unordered_set<Collision2D*> candidates;
    tree.retrieve(candidates, obj);
    
    obj->info = Collision2DInfos();

    float smallestPenetration = std::numeric_limits<float>::max();

    for (Collision2D* other : candidates) {
        if (other == obj) continue;

        Collision2DInfos info = CollisionDetection(obj, other);
        if (info.isColliding) {
            obj->info.isColliding = true;
            obj->info.Colliders.push_back(other);

            if (other->PHYSICS_PARENT) {
                // Take The first MTV
                obj->info.MTV += info.MTV;
                obj->info.ContactPoints.insert(obj->info.ContactPoints.end(), info.ContactPoints.begin(), info.ContactPoints.end());
                obj->info.PhysicsColliders.push_back(other);
            }
        }
        if (info.isPhysicsColliding) obj->info.isPhysicsColliding = true;
    }
}
