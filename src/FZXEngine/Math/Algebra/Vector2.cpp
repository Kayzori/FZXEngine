#include "Vector2.h"

#include <cmath>

// Constructors

Vector2::Vector2() : x(0), y(0) {}
Vector2::Vector2(float x, float y) : x(x), y(y) {}
Vector2::Vector2(float scalar) : x(scalar), y(scalar) {}
Vector2::Vector2(const Vector2& other) : x(other.x), y(other.y) {}

// Arithmetic operators

Vector2 Vector2::operator+(const Vector2& other) const {
    return Vector2(x + other.x, y + other.y);
}

Vector2 Vector2::operator-(const Vector2& other) const {
    return Vector2(x - other.x, y - other.y);
}

Vector2 Vector2::operator*(const float scalar) const {
    return Vector2(x * scalar, y * scalar);
}

Vector2 Vector2::operator/(const float scalar) const {
    return Vector2(x / scalar, y / scalar);
}

// Assignment operators

Vector2& Vector2::operator+=(const Vector2& other) {
    x += other.x;
    y += other.y;
    return *this;
}

Vector2& Vector2::operator-=(const Vector2& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

Vector2& Vector2::operator*=(const float scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}

Vector2& Vector2::operator/=(const float scalar) {
    x /= scalar;
    y /= scalar;
    return *this;
}

// Relational operators

bool Vector2::operator==(const Vector2& other) const {
    return x == other.x && y == other.y;
}

bool Vector2::operator!=(const Vector2& other) const {
    return !(*this == other);
}
