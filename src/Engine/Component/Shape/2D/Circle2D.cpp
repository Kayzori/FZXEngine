#include <Engine/Component/Shape/2D/Circle2D.hpp>

Circle2D::Circle2D(float _radius, int _points):
radius(_radius),
points(_points) 
{
    if (points < 3) points = 3;
}

std::vector<glm::vec2> Circle2D::getVertices() {
    std::vector<glm::vec2> vertices;
    vertices.reserve(points);

    float angle_step = 2.0f * PI / points;

    for (int i = 0; i < points; ++i) {
        float angle = i * angle_step;
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        vertices.emplace_back(x, y);
    }

    return vertices;
}
