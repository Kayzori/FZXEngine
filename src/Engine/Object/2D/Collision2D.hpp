#pragma once
#include <Engine/Component/Components.h>
#include "Object2D.hpp"
#include <Engine/Renderer/2D/Renderer2D.hpp>

class Collision2D;

struct Collision2DInfos {
    // Constructor
    Collision2DInfos() :
        isColliding(false),
        Colliders({}),
        isPhysicsColliding(false),
        PhysicsColliders({}),
        MTV({0.0f, 0.0f}),
        ContactPoints({})
    {}

    // Properties
    // Standard Collision Infos
    bool isColliding;
    std::vector<Collision2D*> Colliders;

    // Physics Body Collision Infos
    bool isPhysicsColliding;
    std::vector<Collision2D*> PhysicsColliders;
    glm::vec2 MTV; // Minimum translation Vector
    std::vector<glm::vec2> ContactPoints;
};

class Collision2D : public Object2D {
public:
    // Con/De structor
    Collision2D(Shape2D* _shape, glm::vec4 _color = glm::vec4(1, 1, 1, 1));
    ~Collision2D();

    // PhysicsBody (Parent)
    Object2D* PHYSICS_PARENT = nullptr;

    // Properties
    Shape2D* shape;
    glm::vec4 color;
    Collision2DInfos info;

    // Methods
    std::vector<glm::vec2> getVertices();

    void OnUpdate(float delta) override;
    void OnDraw() override;
};
