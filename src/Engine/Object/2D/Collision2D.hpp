#pragma once
#include <Engine/Component/Component.h>
#include <Engine/Object/2D/Object2D.hpp>
#include <Engine/Renderer/2D/Renderer2D.hpp>


class Collision2D : Object2D
{
public:
    Transform2D* transform;
    Shape2D* shape;
    glm::vec4 color;

    std::vector<glm::vec2> getVertices();
    bool isColliding();
    void OnDraw() override;
    
    Collision2D(Shape2D* _shape, glm::vec4 _color = glm::vec4(1, 1, 1, 1));
    ~Collision2D();
};
