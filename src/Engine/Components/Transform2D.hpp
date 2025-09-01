#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <Math/Math.hpp>

class Transform2D {
public:
    glm::vec2 position{0.0f, 0.0f};
    glm::vec2 offset{0.0f, 0.0f};
    glm::vec2 scale{1.0f, 1.0f};
    float rotation{0.0f};

    void Apply(std::vector<glm::vec2>& points) const {
        float rad = deg2rad(rotation);
        float cosR = std::cos(rad);
        float sinR = std::sin(rad);
        for (glm::vec2& point : points) {
            point += offset;
            point *= scale;

            glm::vec2 rotated{
                point.x * cosR - point.y * sinR,
                point.x * sinR + point.y * cosR
            };
            point = rotated + position;
        }
    }

    Transform2D() = default;
    Transform2D(glm::vec2 _position, glm::vec2 _offset, glm::vec2 _scale, float _rotation)
    :
        position(_position),
        offset(_offset),
        scale(_scale),
        rotation(_rotation)
    {}
    
    ~Transform2D() = default;
};
