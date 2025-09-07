#pragma once
#include "../Object2D.hpp"
#include "../Collision2D.hpp"

class PhysicsBody2D : public Object2D
{
public:
    // Con/De structor
    PhysicsBody2D(Collision2D* _collision): collision(_collision) {
        if (collision) {
            collision->PHYSICS_PARENT = this;
            collision->transform = transform;
        }
    }
    ~PhysicsBody2D() {
        if (collision) {
            delete collision;
        }
    };

    // Properties
    Collision2D* collision;

    // GameLoop
    void OnDraw() override {
        if (collision) collision->OnDraw();
    }
};
