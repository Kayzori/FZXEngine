#include "Matrix3.h"

#include <algorithm>

// Constructors

Matrix3::Matrix3()
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            m[i][j] = 0.0f;
}

Matrix3::Matrix3(const float diagonal)
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
        {
            if (i == j)
                m[i][j] = diagonal;
            else
                m[i][j] = 0.0f;
        }
}

Matrix3::Matrix3(const float m00, const float m01, const float m02,
           const float m10, const float m11, const float m12,
           const float m20, const float m21, const float m22)
{
    m[0][0] = m00; m[0][1] = m01; m[0][2] = m02;
    m[1][0] = m10; m[1][1] = m11; m[1][2] = m12;
    m[2][0] = m20; m[2][1] = m21; m[2][2] = m22;
}

Matrix3::Matrix3(const float matrix[3][3])
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            m[i][j] = matrix[i][j];
}

Matrix3::Matrix3(const Matrix3& other)
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            m[i][j] = other.m[i][j];
}

// Methods

Matrix3 Matrix3::transposed() const
{
    Matrix3 result;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            result.m[i][j] = m[j][i];
    return result;
}

Matrix3 Matrix3::inverted() const
{
    Matrix3 result;
    float det = determinant();
    if (det == 0.0f)
        return result; // Return zero matrix if not invertible

    float invDet = 1.0f / det;

    result.m[0][0] = (m[1][1] * m[2][2] - m[1][2] * m[2][1]) * invDet;
    result.m[0][1] = (m[0][2] * m[2][1] - m[0][1] * m[2][2]) * invDet;
    result.m[0][2] = (m[0][1] * m[1][2] - m[0][2] * m[1][1]) * invDet;

    result.m[1][0] = (m[1][2] * m[2][0] - m[1][0] * m[2][2]) * invDet;
    result.m[1][1] = (m[0][0] * m[2][2] - m[0][2] * m[2][0]) * invDet;
    result.m[1][2] = (m[0][2] * m[1][0] - m[0][0] * m[1][2]) * invDet;

    result.m[2][0] = (m[1][0] * m[2][1] - m[1][1] * m[2][0]) * invDet;
    result.m[2][1] = (m[0][1] * m[2][0] - m[0][0] * m[2][1]) * invDet;
    result.m[2][2] = (m[0][0] * m[1][1] - m[0][1] * m[1][0]) * invDet;

    return result;
}

float Matrix3::determinant() const
{
    return m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) -
           m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
           m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
}

// Arithmetic operators

Matrix3 Matrix3::operator+(const Matrix3& other) const
{
    Matrix3 result;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            result.m[i][j] = m[i][j] + other.m[i][j];
    return result;
}

Matrix3 Matrix3::operator-(const Matrix3& other) const
{
    Matrix3 result;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            result.m[i][j] = m[i][j] - other.m[i][j];
    return result;
}

Matrix3 Matrix3::operator*(const Matrix3& other) const
{
    Matrix3 result;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            result.m[i][j] = m[i][0] * other.m[0][j] +
                             m[i][1] * other.m[1][j] +
                             m[i][2] * other.m[2][j];
    return result;
}

Matrix3 Matrix3::operator*(const float scalar) const
{
    Matrix3 result;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            result.m[i][j] = m[i][j] * scalar;
    return result;
}

Matrix3 Matrix3::operator/(const float scalar) const
{
    Matrix3 result;
    if (scalar == 0.0f)
        return result; // Return zero matrix if division by zero

    float invScalar = 1.0f / scalar;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            result.m[i][j] = m[i][j] * invScalar;
    return result;
}

// Assignment operators

Matrix3& Matrix3::operator+=(const Matrix3& other)
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            m[i][j] += other.m[i][j];
    return *this;
}

Matrix3& Matrix3::operator-=(const Matrix3& other)
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            m[i][j] -= other.m[i][j];
    return *this;
}

Matrix3& Matrix3::operator*=(const Matrix3& other)
{
    *this = *this * other;
    return *this;
}

Matrix3& Matrix3::operator*=(const float scalar)
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            m[i][j] *= scalar;
    return *this;
}

Matrix3& Matrix3::operator/=(const float scalar)
{
    if (scalar == 0.0f)
        return *this; // Do nothing if division by zero

    float invScalar = 1.0f / scalar;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            m[i][j] *= invScalar;
    return *this;
}

// Relational operators

bool Matrix3::operator==(const Matrix3& other) const
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (m[i][j] != other.m[i][j])
                return false;
    return true;
}

bool Matrix3::operator!=(const Matrix3& other) const
{
    return !(*this == other);
}

// Multiplications
Vector3 Matrix3::operator*(const Vector3& v) const
{
    return {
        m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
        m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
        m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z
    };
}

// Indexing operator

float Matrix3::operator[](const int index) const
{
    std::clamp(index, 0, 8);

    int row = index / 3;
    int col = index % 3;
    return m[row][col];
}
