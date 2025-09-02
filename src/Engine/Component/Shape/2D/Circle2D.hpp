#pragma once
#include <Engine/Component/Shape/2D/Shape2D.hpp>

class Circle2D : public Shape2D
{
public:
    float radius{0.0f};
    int points{3};
    std::vector<glm::vec2> getVertices() override;

    //=== Constructors ===
    Circle2D(float _radius, int _points);
    Circle2D() = default;
};
