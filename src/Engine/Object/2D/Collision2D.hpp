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

    static glm::vec2 getGlobalContactPoint(const std::vector<glm::vec2> _ContactPoints) {
        glm::vec2 GlobalContactPoint = glm::vec2(0.0f);
        for (const glm::vec2 ContactPoint : _ContactPoints) {
            GlobalContactPoint += ContactPoint;
        }
        GlobalContactPoint /= _ContactPoints.size();
        return GlobalContactPoint;
    }
};

class Collision2D : public Object2D {
public:
    // Con/De structor
    Collision2D(
        Shape2D* _shape,
        glm::vec4 _color = {1.0f, 1.0f, 1.0f, 1.0f},
        glm::vec4 _outline_color = {0.0f, 0.0f, 0.0f, 1.0f},
        glm::vec4 _colliding_color = {1.0f, 0.0f, 0.0f, 1.0f}
    );
    ~Collision2D();

    // PhysicsBody (Parent)
    Object2D* PHYSICS_PARENT = nullptr;

    // Properties
    Shape2D* shape;
    glm::vec4 color;
    glm::vec4 outline_color;
    glm::vec4 colliding_color;
    Collision2DInfos info;

    // Methods
    std::vector<glm::vec2> getVertices();

    void OnUpdate(float delta) override;
    void OnDraw() override;
};
