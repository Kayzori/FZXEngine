#pragma once

#include "../Vector/Vector4.h"

class Matrix4
{
public:
    // Properties
    float m[4][4];

    // Constructors
    Matrix4();
    Matrix4(const float m00, const float m01, const float m02, const float m03,
            const float m10, const float m11, const float m12, const float m13,
            const float m20, const float m21, const float m22, const float m23,
            const float m30, const float m31, const float m32, const float m33);
    Matrix4(const float matrix[4][4]);
    Matrix4(const Matrix4& other);
    explicit Matrix4(const float diagonal);

    // Arithmetic operators
    Matrix4 operator+(const Matrix4& other) const;
    Matrix4 operator-(const Matrix4& other) const;
    Matrix4 operator*(const Matrix4& other) const;
    Matrix4 operator*(const float scalar) const;
    Matrix4 operator/(const float scalar) const;

    // Assignment operators
    Matrix4& operator+=(const Matrix4& other);
    Matrix4& operator-=(const Matrix4& other);
    Matrix4& operator*=(const Matrix4& other);
    Matrix4& operator*=(const float scalar);
    Matrix4& operator/=(const float scalar);

    // Relational operators
    bool operator==(const Matrix4& other) const;
    bool operator!=(const Matrix4& other) const;

    // Multiplications
    Vector4 operator*(const Vector4& vector) const;

    // Indexing operator
    float operator[](const int index) const;
};
