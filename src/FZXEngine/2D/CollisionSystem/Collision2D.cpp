#include "Collision2D.h"

// Constructors and Destructors
Collision2D::Collision2D(Transform2D* transform, Shape2D* shape, void* parent) :
Object2D(transform),
parent(parent)
{
    this->shape = new Shape2D(transform, shape);
}

Collision2D::Collision2D(Shape2D* shape, void* parent) :
Object2D(),
parent(parent)
{
    this->shape = new Shape2D(this->transform, shape);
}

Collision2D::~Collision2D() {
    if (shape) delete shape;
}