#pragma once
#include "../Object2D.hpp"
#include "../Collision2D.hpp"

class PhysicsBody2D : public Object2D
{
public:
    // Con/De structor
    PhysicsBody2D(Collision2D* _collision);
    ~PhysicsBody2D();

    // Properties
    Collision2D* collision;

    // GameLoop
    virtual void OnDraw() override;
};
