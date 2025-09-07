#pragma once
#include "Object2D.hpp"

class Collision2D;

struct Collision2DInfos {
    // Constructor
    Collision2DInfos() :
        isColliding(false),
        Colliders({}),
        isPhysicsColliding(false),
        PhysicsColliders({}),
        MTV({}),
        ContactPoints({})
    {}

    // Properties
    // Standard Collision Infos
    bool isColliding;
    std::vector<Collision2D*> Colliders;

    // Physics Body Collision Infos
    bool isPhysicsColliding;
    std::vector<Collision2D*> PhysicsColliders;
    std::unordered_map<Collision2D*, glm::vec2> MTV; // Minimum translation Vector
    std::unordered_map<Collision2D*, std::vector<glm::vec2>> ContactPoints;
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
    std::vector<Edge2D> getEdges();
    glm::vec2 getCenter();
    AABB getBounds();
    bool hasPoint(glm::vec2 point);

    void OnUpdate(float delta) override;
    void OnDraw() override;
};
