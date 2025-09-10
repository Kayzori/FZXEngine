#include "Collision2D.hpp"
#include <Engine/Servers/PhysicsServer/PhysicsServer.hpp>

// The Collision Class Isn't organized that much due to so methods must be from Shape2D
// and not this object
// TODO: Update Shape2D as an Object2D

// Con/De structor
Collision2D::Collision2D(
    Shape2D* _shape,
    glm::vec4 _color,
    glm::vec4 _outline_color,
    glm::vec4 _colliding_color
):
shape(_shape),
color(_color),
outline_color(_outline_color),
colliding_color(_colliding_color)
{
    PhysicsServer::CollisionSystem::SpatialGrid->AddObject(this);
}

Collision2D::~Collision2D() {
    PhysicsServer::CollisionSystem::SpatialGrid->RemoveObject(this);
    if (shape) delete shape;
}

// Methods
std::vector<glm::vec2> Collision2D::getVertices() {
    return transform->Apply(shape->vertices);
}

std::vector<Edge2D> Collision2D::getEdges() {
    std::vector<glm::vec2> verts = transform->Apply(shape->vertices);
    std::vector<Edge2D> edges;
    for (int i = 0; i < int(verts.size()); i++) {
        edges.push_back(Edge2D(verts[i], verts[(i + 1) % verts.size()]));
    }
    return edges;
}

glm::vec2 Collision2D::getCenter() {
    return transform->Apply({shape->center})[0];
}

AABB Collision2D::getBounds() {
    return AABB(getVertices());
}

bool Collision2D::hasPoint(glm::vec2 point) {
    bool inside = false;
    std::vector<glm::vec2> polygon = getVertices();
    int n = polygon.size();
    for (int i = 0, j = n - 1; i < n; j = i++) {
        if (((polygon[i].y > point.y) != (polygon[j].y > point.y)) &&
            (point.x < (polygon[j].x - polygon[i].x) * (point.y - polygon[i].y) / 
                      (polygon[j].y - polygon[i].y) + polygon[i].x)) {
            inside = !inside;
        }
    }
    return inside;
}

void Collision2D::OnDraw() {
    std::vector<glm::vec2> verts = getVertices();
    if (info.isColliding) Renderer2D::DrawPolygon(verts, colliding_color);
    else Renderer2D::DrawPolygon(verts, color);
    Renderer2D::DrawLines(verts, outline_color);
    Renderer2D::DrawLines({verts[0], transform->position}, outline_color);
    for (auto& [other, contacts] : info.ContactPoints) {
        Renderer2D::DrawPoints(contacts, {1, 0, 0, 1}, 10.0f);
    }
}
