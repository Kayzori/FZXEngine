#pragma once
#include <Math/Math.hpp>
#include <Engine/Component/Component.hpp>

// Base Shape
class Shape2D : Component
{
public:
    std::vector<glm::vec2> vertices;
    std::vector<Edge2D> edges;
    glm::vec2 center;
    virtual void computeVertices() {};
    virtual void computeEdges() {};
    virtual AABB getAABB() {return {};};
    void computeCenter()
    {
        if (vertices.size()) computeVertices();
        const size_t numPoints = vertices.size();
        if (numPoints == 0) center = glm::vec2(0.0f);
        if (numPoints == 1) center = vertices[0];
        if (numPoints == 2) center = (vertices[0] + vertices[1]) * 0.5f;
        
        float X = 0.0f;
        float Y = 0.0f;
        float area = 0.0f;
        
        for (size_t i = 0; i < numPoints; i++) {
            size_t j = (i + 1) % numPoints;
            
            const float xi = vertices[i].x;
            const float yi = vertices[i].y;
            const float xj = vertices[j].x;
            const float yj = vertices[j].y;
            
            const float cross = xi * yj - xj * yi;
            area += cross;
            
            X += (xi + xj) * cross;
            Y += (yi + yj) * cross;
        }
        
        area *= 0.5f;
        const float absArea = std::abs(area);
        
        if (absArea < 1e-8f) {
            glm::vec2 sum(0.0f);
            for (const auto& p : vertices) sum += p;
            center = sum / static_cast<float>(numPoints);
        }
        
        const float inv6Area = 1.0f / (6.0f * area);
        center = glm::vec2(X * inv6Area, Y * inv6Area);
    }

    Shape2D() = default;
    virtual ~Shape2D() = default;
};

// Box2D
class Box2D : public Shape2D
{
public:
    // Constructors
    Box2D(float _width, float _height) :
    w(_width),
    h(_height)
    {
        computeVertices();
        computeCenter();
    }

    Box2D(float sq_size) :
    w(sq_size),
    h(sq_size)
    {
        computeVertices();
        computeCenter();
    }

    Box2D(glm::vec2 size) :
    w(size.x),
    h(size.y)
    {
        computeVertices();
        computeCenter();
    }

    // Properties
    float w;
    float h;

    // Method
    void computeVertices() override
    {
        float hx = w/2.0f;
        float hy = h/2.0f;
        vertices = {
            {-hx, -hy},
            {hx, -hy},
            {hx, hy},
            {-hx, hy}
        };
    }

    void computeEdges() override
    {
        if (vertices.size()) computeVertices();
        edges.clear();
        for (int i{0}; i < 4; i++)
            edges.push_back({vertices[i], vertices[(i + 1) % 4]});
    }

    AABB getAABB() override {
        return AABB({0.0f, 0.0f}, {w, h});
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
        computeVertices();
        computeCenter();
    }

    // Properties
    float radius;
    int points;

    // Method
    void computeVertices() override
    {
        vertices.clear();
        vertices.reserve(points);

        float angle_step = 2.0f * PI / points;

        for (int i = 0; i < points; ++i) {
            float angle = i * angle_step;
            float x = radius * cos(angle);
            float y = radius * sin(angle);
            vertices.emplace_back(x, y);
        }
    }

    AABB getAABB() override {
        return AABB({0.0f, 0.0f}, radius);
    }

    void computeEdges() override
    {
        if (vertices.size()) computeVertices();
        edges.clear();
        int size = vertices.size();
        for (int i{0}; i < size; i++)
            edges.push_back({vertices[i], vertices[(i + 1) % size]});
    }
};
