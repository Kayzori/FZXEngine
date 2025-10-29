#pragma once

#include "../Vector/Vector2.h"

class Matrix2
{
public:
    // Properties
    float m[2][2];

    // Constructors
    Matrix2();
    Matrix2(const float m00, const float m01,
            const float m10, const float m11);
    Matrix2(const float matrix[2][2]);
    Matrix2(const Matrix2& other);
    explicit Matrix2(const float diagonal);

    // Arithmetic operators
    Matrix2 operator+(const Matrix2& other) const;
    Matrix2 operator-(const Matrix2& other) const;
    Matrix2 operator*(const Matrix2& other) const;
    Matrix2 operator*(const float scalar) const;
    Matrix2 operator/(const float scalar) const;

    // Assignment operators
    Matrix2& operator+=(const Matrix2& other);
    Matrix2& operator-=(const Matrix2& other);
    Matrix2& operator*=(const Matrix2& other);
    Matrix2& operator*=(const float scalar);
    Matrix2& operator/=(const float scalar);

    // Relational operators
    bool operator==(const Matrix2& other) const;
    bool operator!=(const Matrix2& other) const;

    // Multiplications
    Vector2 operator*(const Vector2& vector) const;

    // Indexing operator
    float operator[](const int index) const;
};
