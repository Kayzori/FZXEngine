#pragma once
#include <Math/Math.hpp>
#include <Engine/Component/Shape/Shape.hpp>

class Shape2D : Shape
{
public:
    inline virtual std::vector<glm::vec2> getVertices() {return {};};
    Shape2D() = default;
    virtual ~Shape2D() = default;
};
