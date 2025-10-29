#pragma once

#include "Matrix2.h"
#include "Matrix3.h"
#include "Matrix4.h"

namespace Math
{
    namespace Algebra {
        Matrix2 transpose(const Matrix2& mat2);
        Matrix3 transpose(const Matrix3& mat3);
        Matrix4 transpose(const Matrix4& mat4);
        
        Matrix2 inverted(const Matrix2& mat2);
        Matrix3 inverted(const Matrix3& mat3);
        Matrix4 inverted(const Matrix4& mat4);

        float determinant(const Matrix2& mat2);
        float determinant(const Matrix3& mat3);
        float determinant(const Matrix4& mat4);

        Matrix4 ortho(float left, float right, float bottom, float top, float z_near, float z_far);
        Matrix4 ortho(float left, float right, float bottom, float top);
    }
}