#include "Collision2D.hpp"
#include <Engine/Servers/PhysicsServer/PhysicsServer.hpp>

// Con/De structor
Collision2D::Collision2D(Shape2D* _shape, glm::vec4 _color):
shape(_shape),
color(_color)
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
    Renderer2D::DrawPolygon(getVertices(), color);
}
