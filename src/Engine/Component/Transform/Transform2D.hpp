#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <Math/Math.hpp>

#include <Engine/Component/Transform/Transform.hpp>

class Transform2D : Transform {
public:
    glm::vec2 offset{0.0f, 0.0f};
    glm::vec2 position{0.0f, 0.0f};
    glm::vec2 scale{1.0f, 1.0f};
    float rotation{0.0f};

    std::vector<glm::vec2> Apply(std::vector<glm::vec2> points) const {
        std::vector<glm::vec2> result;
        float rad = deg2rad(rotation);
        float cosR = std::cos(rad);
        float sinR = std::sin(rad);
        for (glm::vec2 point : points) {
            point *= scale;
            glm::vec2 rotated{
                point.x * cosR - point.y * sinR,
                point.x * sinR + point.y * cosR
            };
            point = rotated + position + offset;
            result.push_back(point);
        }
        return result;
    }


    Transform2D() = default;
    Transform2D(glm::vec2 _offset, glm::vec2 _position, glm::vec2 _scale, float _rotation)
    :
        offset(_offset),
        position(_position),
        scale(_scale),
        rotation(_rotation)
    {}
    
    ~Transform2D() = default;
};
