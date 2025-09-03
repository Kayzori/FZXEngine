#pragma once
#include <Math/Math.hpp>
#include <Engine/Component/Component.hpp>

class Transform2D : Component {
public:
    glm::vec2 offset;
    glm::vec2 position;
    glm::vec2 scale;
    float rotation;

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

    Transform2D(glm::vec2 _offset = {0.0f, 0.0f}, glm::vec2 _position = {0.0f, 0.0f}, glm::vec2 _scale = {1.0f, 1.0f}, float _rotation = 0.0f)
    :
        offset(_offset),
        position(_position),
        scale(_scale),
        rotation(_rotation)
    {}
    ~Transform2D() = default;
};
