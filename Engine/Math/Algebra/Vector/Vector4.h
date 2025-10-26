#pragma once

class Vector4
{
public:
    float x;
    float y;
    float z;
    float w;

    // Constructors
    Vector4();
    Vector4(float x, float y, float z, float w);
    Vector4(float scalar);
    Vector4(const Vector4& other);

    // Arethmetic operators
    Vector4 operator+(const Vector4& other) const;
    Vector4 operator-(const Vector4& other) const;
    Vector4 operator*(const float scalar) const;
    Vector4 operator/(const float scalar) const;

    // Assignment operators
    Vector4& operator+=(const Vector4& other);
    Vector4& operator-=(const Vector4& other);
    Vector4& operator*=(const float scalar);
    Vector4& operator/=(const float scalar);

    // Relational operators
    bool operator==(const Vector4& other) const;
    bool operator!=(const Vector4& other) const;
};
