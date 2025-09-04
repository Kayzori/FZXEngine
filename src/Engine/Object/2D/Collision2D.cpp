#include "Collision2D.hpp"
#include <Engine/Servers/PhysicsServer/PhysicsServer.hpp>

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
    Physics2DServer::CollisionSystem::InsertCollision(this);
}

Collision2D::~Collision2D() {
    Physics2DServer::CollisionSystem::DeleteCollision(this);
    if (shape) delete shape;
}

// Methods
std::vector<glm::vec2> Collision2D::getVertices() {
    return transform->Apply(shape->getVertices());
}

// GameLoop
void Collision2D::OnUpdate(float delta) {
    Physics2DServer::CollisionSystem::UpdateCollisionInfos(this);
}

void Collision2D::OnDraw() {
    std::vector<glm::vec2> verts = getVertices();
    if (info.isColliding) Renderer2D::DrawPolygon(verts, colliding_color);
    else Renderer2D::DrawPolygon(verts, color);
    Renderer2D::DrawLines(verts, outline_color);
    Renderer2D::DrawLines({verts[0], transform->position}, outline_color);
}
