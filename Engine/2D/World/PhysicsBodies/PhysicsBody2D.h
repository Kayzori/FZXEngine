#pragma once

#include "../../Node2D.h"
#include "../../CollisionSystem/Collision2D.h"

class PhysicsBody2D : public Node2D
{
public:
    // Properties
    Collision2D* collision;
    
    // Constructors and Destructors
    PhysicsBody2D(Collision2DSystem* col_sys, Collision2D* _collision):
        Node2D()
    {
        collision = new Collision2D(col_sys, _collision);
    }
    PhysicsBody2D(Transform2D* transform, Collision2DSystem* col_sys, Collision2D* _collision):
        Node2D(transform)
    {
        collision = new Collision2D(transform, col_sys, _collision);
    }
    ~PhysicsBody2D()
    {
        delete collision;
    }
};
