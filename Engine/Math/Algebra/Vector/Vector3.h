#pragma once

class Vector3
{
public:
    float x;
    float y;
    float z;

    // Constructors
    Vector3();
    Vector3(float x, float y, float z);
    Vector3(float scalar);
    Vector3(const Vector3& other);
    
    // Unary operators
    Vector3 operator+() const;
    Vector3 operator-() const;

    // Arithmetic operators
    Vector3 operator+(const Vector3& other) const;
    Vector3 operator-(const Vector3& other) const;
    Vector3 operator*(const float scalar) const;
    Vector3 operator/(const float scalar) const;

    // Assignment operators
    Vector3& operator+=(const Vector3& other);
    Vector3& operator-=(const Vector3& other);
    Vector3& operator*=(const float scalar);
    Vector3& operator/=(const float scalar);

    // Relational operators
    bool operator==(const Vector3& other) const;
    bool operator!=(const Vector3& other) const;
};
