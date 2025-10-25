#include "Vector4.h"

#include <cmath>

// Constructors

Vector4::Vector4() : x(0), y(0), z(0), w(0) {}

Vector4::Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

Vector4::Vector4(float scalar) : x(scalar), y(scalar), z(scalar), w(scalar) {}

Vector4::Vector4(const Vector4& other) : x(other.x), y(other.y), z(other.z), w(other.w) {}

// Arithmetic operators

Vector4 Vector4::operator+(const Vector4& other) const
{
    return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
}

Vector4 Vector4::operator-(const Vector4& other) const
{
    return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
}

Vector4 Vector4::operator*(const float scalar) const
{
    return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
}

Vector4 Vector4::operator/(const float scalar) const
{
    return Vector4(x / scalar, y / scalar, z / scalar, w / scalar);
}

// Assignment operators

Vector4& Vector4::operator+=(const Vector4& other)
{
    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;
    return *this;
}

Vector4& Vector4::operator-=(const Vector4& other)
{
    x -= other.x;
    y -= other.y;
    z -= other.z;
    w -= other.w;
    return *this;
}

Vector4& Vector4::operator*=(const float scalar)
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
    w *= scalar;
    return *this;
}

Vector4& Vector4::operator/=(const float scalar)
{
    x /= scalar;
    y /= scalar;
    z /= scalar;
    w /= scalar;
    return *this;
}

// Relational operators

bool Vector4::operator==(const Vector4& other) const
{
    return x == other.x && y == other.y && z == other.z && w == other.w;
}

bool Vector4::operator!=(const Vector4& other) const
{
    return !(*this == other);
}
