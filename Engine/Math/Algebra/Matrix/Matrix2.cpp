#include "Matrix2.h"

#include <algorithm>

// Constructors
Matrix2::Matrix2()
{
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            m[i][j] = 0.0f;
}

Matrix2::Matrix2(const float diagonal)
{
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
        {
            if (i == j)
                m[i][j] = diagonal;
            else
                m[i][j] = 0.0f;
        }
}

Matrix2::Matrix2(const float m00, const float m01,
                const float m10, const float m11)
{
    m[0][0] = m00; m[0][1] = m01;
    m[1][0] = m10; m[1][1] = m11;
}

Matrix2::Matrix2(const float matrix[2][2])
{
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            m[i][j] = matrix[i][j];
}

Matrix2::Matrix2(const Matrix2& other)
{
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            m[i][j] = other.m[i][j];
}

// Arithmetic operators

Matrix2 Matrix2::operator+(const Matrix2& other) const
{
    Matrix2 result;
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            result.m[i][j] = m[i][j] + other.m[i][j];
    return result;
}

Matrix2 Matrix2::operator-(const Matrix2& other) const
{
    Matrix2 result;
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            result.m[i][j] = m[i][j] - other.m[i][j];
    return result;
}

Matrix2 Matrix2::operator*(const Matrix2& other) const
{
    Matrix2 result;
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            result.m[i][j] = m[i][0] * other.m[0][j] +
                             m[i][1] * other.m[1][j];
    return result;
}

Matrix2 Matrix2::operator*(const float scalar) const
{
    Matrix2 result;
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            result.m[i][j] = m[i][j] * scalar;
    return result;
}

Matrix2 Matrix2::operator/(const float scalar) const
{
    Matrix2 result;
    if (scalar == 0.0f)
        return result; // Return zero matrix if division by zero

    float invScalar = 1.0f / scalar;
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            result.m[i][j] = m[i][j] * invScalar;
    return result;
}

// Assignment operators

Matrix2& Matrix2::operator+=(const Matrix2& other)
{
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            m[i][j] += other.m[i][j];
    return *this;
}

Matrix2& Matrix2::operator-=(const Matrix2& other)
{
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            m[i][j] -= other.m[i][j];
    return *this;
}

Matrix2& Matrix2::operator*=(const Matrix2& other)
{
    *this = *this * other;
    return *this;
}

Matrix2& Matrix2::operator*=(const float scalar)
{
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            m[i][j] *= scalar;
    return *this;
}

Matrix2& Matrix2::operator/=(const float scalar)
{
    if (scalar == 0.0f)
        return *this; // Do nothing if division by zero

    float invScalar = 1.0f / scalar;
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            m[i][j] *= invScalar;
    return *this;
}

// Relational operators

bool Matrix2::operator==(const Matrix2& other) const
{
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            if (m[i][j] != other.m[i][j])
                return false;
    return true;
}

bool Matrix2::operator!=(const Matrix2& other) const
{
    return !(*this == other);
}

// Multiplications

Vector2 Matrix2::operator*(const Vector2& v) const
{
    return {
        m[0][0] * v.x + m[0][1] * v.y,
        m[1][0] * v.x + m[1][1] * v.y
    };
}

// Indexing operator

float Matrix2::operator[](const int index) const
{
    std::clamp(index, 0, 3);

    int row = index / 2;
    int col = index % 2;
    return m[row][col];
}
