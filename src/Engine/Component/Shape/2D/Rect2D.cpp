#include <Engine/Component/Shape/2D/Rect2D.hpp>

Rect2D::Rect2D(float _width, float _height):
width(_width),
height(_height)
{}

Rect2D::Rect2D(glm::vec2 size):
width(size.x),
height(size.y)
{}

std::vector<glm::vec2> Rect2D::getVertices() {
    float hx = width/2.0f;
    float hy = height/2.0f;
    return {
        {-hx, -hy},
        {hx, -hy},
        {hx, hy},
        {-hx, hy}
    };
}