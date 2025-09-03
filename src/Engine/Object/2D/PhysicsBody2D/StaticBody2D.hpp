#pragma once
#include "PhysicsBody2D.hpp"

class StaticBody2D : public PhysicsBody2D
{
public:
    // Constructor
    StaticBody2D(Collision2D* _collision);
};
