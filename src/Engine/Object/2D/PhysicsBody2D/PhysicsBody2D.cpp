#include "PhysicsBody2D.hpp"

// Con/De structor
PhysicsBody2D::PhysicsBody2D(Collision2D* _collision) :
collision(_collision)
{
    if (collision) {
        collision->PHYSICS_PARENT = this;
        collision->transform = transform;
    }
}

PhysicsBody2D::~PhysicsBody2D() {
    if (collision) delete collision;
}

//GameLoop
void PhysicsBody2D::OnDraw() {
    if (collision) collision->OnDraw();
}
