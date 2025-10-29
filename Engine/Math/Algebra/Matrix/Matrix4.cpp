#include "Matrix4.h"
#include <algorithm>

// Constructors

Matrix4::Matrix4()
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            m[i][j] = 0.0f;
}

Matrix4::Matrix4(const float diagonal)
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            m[i][j] = (i == j) ? diagonal : 0.0f;
}

Matrix4::Matrix4(
    const float m00, const float m01, const float m02, const float m03,
    const float m10, const float m11, const float m12, const float m13,
    const float m20, const float m21, const float m22, const float m23,
    const float m30, const float m31, const float m32, const float m33)
{
    m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
    m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
    m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
    m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
}

Matrix4::Matrix4(const float matrix[4][4])
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            m[i][j] = matrix[i][j];
}

Matrix4::Matrix4(const Matrix4& other)
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            m[i][j] = other.m[i][j];
}

// Arithmetic operators

Matrix4 Matrix4::operator+(const Matrix4& other) const
{
    Matrix4 result;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            result.m[i][j] = m[i][j] + other.m[i][j];
    return result;
}

Matrix4 Matrix4::operator-(const Matrix4& other) const
{
    Matrix4 result;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            result.m[i][j] = m[i][j] - other.m[i][j];
    return result;
}

Matrix4 Matrix4::operator*(const Matrix4& other) const
{
    Matrix4 result;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
        {
            result.m[i][j] =
                m[i][0] * other.m[0][j] +
                m[i][1] * other.m[1][j] +
                m[i][2] * other.m[2][j] +
                m[i][3] * other.m[3][j];
        }
    return result;
}

Matrix4 Matrix4::operator*(const float scalar) const
{
    Matrix4 result;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            result.m[i][j] = m[i][j] * scalar;
    return result;
}

Matrix4 Matrix4::operator/(const float scalar) const
{
    Matrix4 result;
    if (scalar == 0.0f)
        return result; // return zero matrix

    float inv = 1.0f / scalar;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            result.m[i][j] = m[i][j] * inv;
    return result;
}

// Assignment operators

Matrix4& Matrix4::operator+=(const Matrix4& other)
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            m[i][j] += other.m[i][j];
    return *this;
}

Matrix4& Matrix4::operator-=(const Matrix4& other)
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            m[i][j] -= other.m[i][j];
    return *this;
}

Matrix4& Matrix4::operator*=(const Matrix4& other)
{
    *this = *this * other;
    return *this;
}

Matrix4& Matrix4::operator*=(const float scalar)
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            m[i][j] *= scalar;
    return *this;
}

Matrix4& Matrix4::operator/=(const float scalar)
{
    if (scalar == 0.0f)
        return *this;

    float inv = 1.0f / scalar;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            m[i][j] *= inv;
    return *this;
}

// Relational operators

bool Matrix4::operator==(const Matrix4& other) const
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (m[i][j] != other.m[i][j])
                return false;
    return true;
}

bool Matrix4::operator!=(const Matrix4& other) const
{
    return !(*this == other);
}

// Multiplication with Vector3 (assuming homogeneous w = 1)
Vector4 Matrix4::operator*(const Vector4& v) const
{
    float x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w;
    float y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w;
    float z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w;
    float w = m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w;

    return { x, y, z, w };
}

// Indexing operator (flattened)
float Matrix4::operator[](const int index) const
{
    int clamped = std::clamp(index, 0, 15);
    int row = clamped / 4;
    int col = clamped % 4;
    return m[row][col];
}
