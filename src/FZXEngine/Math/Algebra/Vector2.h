#pragma once

class Vector2
{
public:
    float x;
    float y;

    // Constructors
    Vector2();
    Vector2(float x, float y);
    Vector2(float scalar);
    Vector2(const Vector2& other);

    // Arithmetic operators
    Vector2 operator+(const Vector2& other) const;
    Vector2 operator-(const Vector2& other) const;
    Vector2 operator*(const float scalar) const;
    Vector2 operator/(const float scalar) const;

    // Assignment operators
    Vector2& operator+=(const Vector2& other);
    Vector2& operator-=(const Vector2& other);
    Vector2& operator*=(const float scalar);
    Vector2& operator/=(const float scalar);

    // Relational operators
    bool operator==(const Vector2& other) const;
    bool operator!=(const Vector2& other) const;
};
