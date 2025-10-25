#pragma once

#include "Vector3.h"

class Matrix3
{
private:
    float m[3][3];
public:

    // Constructors
    Matrix3();
    Matrix3(const float m00, const float m01, const float m02,
         const float m10, const float m11, const float m12,
         const float m20, const float m21, const float m22);
    Matrix3(const float matrix[3][3]);
    Matrix3(const Matrix3& other);
    explicit Matrix3(const float diagonal);

    // Methods
    Matrix3 transposed() const;
    Matrix3 inverted() const;
    float determinant() const;

    // Arithmetic operators
    Matrix3 operator+(const Matrix3& other) const;
    Matrix3 operator-(const Matrix3& other) const;
    Matrix3 operator*(const Matrix3& other) const;
    Matrix3 operator*(const float scalar) const;
    Matrix3 operator/(const float scalar) const;

    // Assignment operators
    Matrix3& operator+=(const Matrix3& other);
    Matrix3& operator-=(const Matrix3& other);
    Matrix3& operator*=(const Matrix3& other);
    Matrix3& operator*=(const float scalar);
    Matrix3& operator/=(const float scalar);

    // Relational operators
    bool operator==(const Matrix3& other) const;
    bool operator!=(const Matrix3& other) const;

    // Multiplications
    Vector3 operator*(const Vector3& v) const;

    // Indexing operator
    float operator[](const int index) const;
};
