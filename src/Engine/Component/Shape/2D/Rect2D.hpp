#pragma once
#include <Engine/Component/Shape/2D/Shape2D.hpp>

class Rect2D : public Shape2D
{
public:
    float width{0.0f};
    float height{0.0f};
    std::vector<glm::vec2> getVertices() override;

    //=== Constructors ===//
    Rect2D(float _width, float _height);
    Rect2D(glm::vec2 size);
    Rect2D() = default;
};
