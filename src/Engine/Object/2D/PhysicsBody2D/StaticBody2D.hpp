#pragma once
#include "PhysicsBody2D.hpp"

class StaticBody2D : public PhysicsBody2D
{
public:
    // Con/De structor
    StaticBody2D(Collision2D* _collision): PhysicsBody2D(_collision) {}
    ~StaticBody2D() = default;
};
