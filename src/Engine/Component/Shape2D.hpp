#pragma once
#include <Math/Math.hpp>
#include <Engine/Component/Component.hpp>

// Base Shape
class Shape2D : Component
{
public:
    virtual std::vector<glm::vec2> getVertices() {return {};};
    Shape2D() = default;
    virtual ~Shape2D() = default;
};

// Box2D
class Box2D : public Shape2D
{
public:
    // Constructors
    Box2D(float _width, float _height) :
    width(_width),
    height(_height)
    {}

    // Properties
    float width;
    float height;

    // Method
    std::vector<glm::vec2> getVertices() override  {
        float hx = width/2.0f;
        float hy = height/2.0f;
        return {
            {-hx, -hy},
            {hx, -hy},
            {hx, hy},
            {-hx, hy}
        };
    }
};

// Circle2D
class Circle2D : public Shape2D
{
public:
    // Constructor
    Circle2D(float _radius = 0.0f, int _points = 3) :
    radius(_radius),
    points(_points) 
    {
        if (points < 3) points = 3;
    }

    // Properties
    float radius;
    int points;

    // Method
    std::vector<glm::vec2> getVertices()
    {
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
};
