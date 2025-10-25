#include "Vector3.h"

#include <cmath>

// Constructors

Vector3::Vector3() : x(0), y(0), z(0) {}
Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
Vector3::Vector3(float scalar) : x(scalar), y(scalar), z(scalar) {}
Vector3::Vector3(const Vector3& other) : x(other.x), y(other.y), z(other.z) {}

// Arithmetic operators

Vector3 Vector3::operator+(const Vector3& other) const
{
    return Vector3(x + other.x, y + other.y, z + other.z);
}

Vector3 Vector3::operator-(const Vector3& other) const
{
    return Vector3(x - other.x, y - other.y, z - other.z);
}

Vector3 Vector3::operator*(const float scalar) const
{
    return Vector3(x * scalar, y * scalar, z * scalar);
}

Vector3 Vector3::operator/(const float scalar) const
{
    return Vector3(x / scalar, y / scalar, z / scalar);
}

// Assignment operators

Vector3& Vector3::operator+=(const Vector3& other)
{
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

Vector3& Vector3::operator-=(const Vector3& other)
{
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

Vector3& Vector3::operator*=(const float scalar)
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

Vector3& Vector3::operator/=(const float scalar)
{
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
}

// Relational operators

bool Vector3::operator==(const Vector3& other) const
{
    return x == other.x && y == other.y && z == other.z;
}

bool Vector3::operator!=(const Vector3& other) const
{
    return !(*this == other);
}
