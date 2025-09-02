#include <Engine/Object/2D/Collision2D.hpp>
#include <Engine/Servers/PhysicsServer/PhysicsServer.hpp>

Collision2D::Collision2D(Shape2D* _shape, glm::vec4 _color):
shape(_shape),
color(_color)
{
    transform = new Transform2D();
    PhysicsServer::InsertCollision(this);
}

Collision2D::~Collision2D() {
    delete transform;
    delete shape;

    PhysicsServer::DeleteCollision(this);
}

std::vector<glm::vec2> Collision2D::getVertices() {
    return transform->Apply(shape->getVertices());
}

bool Collision2D::isColliding() {
    return PhysicsServer::Collision2DDetection(this);
}

void Collision2D::OnDraw() {
    Renderer2D::DrawPolygon(getVertices(), color);
}
