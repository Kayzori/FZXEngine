#include "Transform2D.h"

#include <cmath>

#define PI 3.14159265358979323846

// Constructors

Transform2D::Transform2D() :
    position(0, 0),
    rotation(0),
    scale(1, 1)
{}

Transform2D::Transform2D(const Vector2& position, float rotation, const Vector2& scale) :
    position(position),
    rotation(rotation),
    scale(scale)
{}

Transform2D::Transform2D(const Matrix3& matrix) {
    setMatrix(matrix);
}

Transform2D::Transform2D(const Transform2D& other) :
    position(other.position),
    rotation(other.rotation),
    scale(other.scale)
{}

// Methods

std::vector<Vector2> Transform2D::apply(const std::vector<Vector2>& points) const {
    std::vector<Vector2> transformed;
    transformed.reserve(points.size());
    for (const auto& point : points) {
        transformed.push_back(apply(point));
    }
    return transformed;
}

Vector2 Transform2D::apply(const Vector2& point) const {
    float rad = rotation * (PI / 180.0f);
    float cosR = std::cos(rad);
    float sinR = std::sin(rad);

    Vector2 scaledPoint = Vector2(point.x * scale.x, point.y * scale.y);
    Vector2 rotatedPoint = Vector2(
        scaledPoint.x * cosR - scaledPoint.y * sinR,
        scaledPoint.x * sinR + scaledPoint.y * cosR
    );
    return Vector2(rotatedPoint.x + position.x, rotatedPoint.y + position.y);
}

std::vector<Vector2> Transform2D::applyTranslation(const std::vector<Vector2>& points) const {
    std::vector<Vector2> transformed;
    transformed.reserve(points.size());
    for (const auto& point : points) {
        transformed.push_back(applyTranslation(point));
    }
    return transformed;
}

Vector2 Transform2D::applyTranslation(const Vector2& point) const {
    return Vector2(point.x + position.x, point.y + position.y);
}

std::vector<Vector2> Transform2D::applyRotation(const std::vector<Vector2>& points) const {
    std::vector<Vector2> transformed;
    transformed.reserve(points.size());
    for (const auto& point : points) {
        transformed.push_back(applyRotation(point));
    }
    return transformed;
}

Vector2 Transform2D::applyRotation(const Vector2& point) const {
    float rad = rotation * (PI / 180.0f);
    float cosR = std::cos(rad);
    float sinR = std::sin(rad);
    return Vector2(
        point.x * cosR - point.y * sinR,
        point.x * sinR + point.y * cosR
    );
}

std::vector<Vector2> Transform2D::applyScale(const std::vector<Vector2>& points) const {
    std::vector<Vector2> transformed;
    transformed.reserve(points.size());
    for (const auto& point : points) {
        transformed.push_back(applyScale(point));
    }
    return transformed;
}

Vector2 Transform2D::applyScale(const Vector2& point) const {
    return Vector2(point.x * scale.x, point.y * scale.y);
}

std::vector<Vector2> Transform2D::applyInverse(const std::vector<Vector2>& points) const {
    std::vector<Vector2> transformed;
    transformed.reserve(points.size());
    for (const auto& point : points) {
        transformed.push_back(applyInverse(point));
    }
    return transformed;
}

Vector2 Transform2D::applyInverse(const Vector2& point) const {
    float rad = -rotation * (PI / 180.0f);
    float cosR = std::cos(rad);
    float sinR = std::sin(rad);

    Vector2 translatedPoint = Vector2(point.x - position.x, point.y - position.y);
    Vector2 rotatedPoint = Vector2(
        translatedPoint.x * cosR - translatedPoint.y * sinR,
        translatedPoint.x * sinR + translatedPoint.y * cosR
    );
    return Vector2(rotatedPoint.x / scale.x, rotatedPoint.y / scale.y);
}

Matrix3 Transform2D::matrix() const {
    float rad = rotation * (PI / 180.0f);
    float cosR = std::cos(rad);
    float sinR = std::sin(rad);

    return Matrix3(
         scale.x * cosR, -scale.y * sinR, position.x ,
         scale.x * sinR,  scale.y * cosR, position.y ,
         0.0f,               0.0f,              1.0f
    );
}

void Transform2D::setMatrix(const Matrix3& matrix) {
    scale.x = std::sqrt(matrix[0] * matrix[0] + matrix[3] * matrix[3]);
    scale.y = std::sqrt(matrix[1] * matrix[1] + matrix[4] * matrix[4]);

    rotation = std::atan2(matrix[3], matrix[0]) * (180.0f / PI);

    position.x = matrix[6];
    position.y = matrix[7];
}

// Relational operators

bool Transform2D::operator==(const Transform2D& other) const {
    return position == other.position && rotation == other.rotation && scale == other.scale;
}

bool Transform2D::operator!=(const Transform2D& other) const {
    return !(*this == other);
}
