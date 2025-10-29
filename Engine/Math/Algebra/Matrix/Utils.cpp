#include "Utils.h"

namespace Math
{
    namespace Algebra
    {
        // Transpose

        Matrix2 transpose(const Matrix2& m)
        {
            Matrix2 r;
            for (int i = 0; i < 2; i++)
                for (int j = 0; j < 2; j++)
                    r.m[i][j] = m.m[j][i];
            return r;
        }

        Matrix3 transpose(const Matrix3& m)
        {
            Matrix3 r;
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                    r.m[i][j] = m.m[j][i];
            return r;
        }

        Matrix4 transpose(const Matrix4& m)
        {
            Matrix4 r;
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    r.m[i][j] = m.m[j][i];
            return r;
        }

        // Determinants

        float determinant(const Matrix2& m)
        {
            return m.m[0][0] * m.m[1][1] - m.m[0][1] * m.m[1][0];
        }

        float determinant(const Matrix3& m)
        {
            return
                m.m[0][0] * (m.m[1][1] * m.m[2][2] - m.m[1][2] * m.m[2][1]) -
                m.m[0][1] * (m.m[1][0] * m.m[2][2] - m.m[1][2] * m.m[2][0]) +
                m.m[0][2] * (m.m[1][0] * m.m[2][1] - m.m[1][1] * m.m[2][0]);
        }

        float determinant(const Matrix4& m)
        {
            const float
                a0 = m.m[0][0] * m.m[1][1] - m.m[1][0] * m.m[0][1],
                a1 = m.m[0][0] * m.m[1][2] - m.m[1][0] * m.m[0][2],
                a2 = m.m[0][0] * m.m[1][3] - m.m[1][0] * m.m[0][3],
                a3 = m.m[0][1] * m.m[1][2] - m.m[1][1] * m.m[0][2],
                a4 = m.m[0][1] * m.m[1][3] - m.m[1][1] * m.m[0][3],
                a5 = m.m[0][2] * m.m[1][3] - m.m[1][2] * m.m[0][3];

            const float b0 = m.m[2][0] * m.m[3][1] - m.m[3][0] * m.m[2][1];
            const float b1 = m.m[2][0] * m.m[3][2] - m.m[3][0] * m.m[2][2];
            const float b2 = m.m[2][0] * m.m[3][3] - m.m[3][0] * m.m[2][3];
            const float b3 = m.m[2][1] * m.m[3][2] - m.m[3][1] * m.m[2][2];
            const float b4 = m.m[2][1] * m.m[3][3] - m.m[3][1] * m.m[2][3];
            const float b5 = m.m[2][2] * m.m[3][3] - m.m[3][2] * m.m[2][3];

            return a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;
        }

        // Inverse

        Matrix2 inverted(const Matrix2& m)
        {
            Matrix2 r;
            float det = determinant(m);
            if (det == 0.0f)
                return Matrix2(); // zero matrix

            float inv = 1.0f / det;
            r.m[0][0] =  m.m[1][1] * inv;
            r.m[0][1] = -m.m[0][1] * inv;
            r.m[1][0] = -m.m[1][0] * inv;
            r.m[1][1] =  m.m[0][0] * inv;
            return r;
        }

        Matrix3 inverted(const Matrix3& m)
        {
            Matrix3 r;
            float det = determinant(m);
            if (det == 0.0f)
                return Matrix3();

            float inv = 1.0f / det;

            r.m[0][0] =  (m.m[1][1] * m.m[2][2] - m.m[2][1] * m.m[1][2]) * inv;
            r.m[0][1] = -(m.m[0][1] * m.m[2][2] - m.m[2][1] * m.m[0][2]) * inv;
            r.m[0][2] =  (m.m[0][1] * m.m[1][2] - m.m[1][1] * m.m[0][2]) * inv;

            r.m[1][0] = -(m.m[1][0] * m.m[2][2] - m.m[2][0] * m.m[1][2]) * inv;
            r.m[1][1] =  (m.m[0][0] * m.m[2][2] - m.m[2][0] * m.m[0][2]) * inv;
            r.m[1][2] = -(m.m[0][0] * m.m[1][2] - m.m[1][0] * m.m[0][2]) * inv;

            r.m[2][0] =  (m.m[1][0] * m.m[2][1] - m.m[2][0] * m.m[1][1]) * inv;
            r.m[2][1] = -(m.m[0][0] * m.m[2][1] - m.m[2][0] * m.m[0][1]) * inv;
            r.m[2][2] =  (m.m[0][0] * m.m[1][1] - m.m[1][0] * m.m[0][1]) * inv;

            return r;
        }

        Matrix4 inverted(const Matrix4& m)
        {
            Matrix4 inv;
            float det = determinant(m);
            if (det == 0.0f)
                return Matrix4();

            float inv_det = 1.0f / det;

            inv.m[0][0] =  m.m[1][1] * m.m[2][2] * m.m[3][3] - m.m[1][1] * m.m[2][3] * m.m[3][2] - m.m[2][1] * m.m[1][2] * m.m[3][3]
                        + m.m[2][1] * m.m[1][3] * m.m[3][2] + m.m[3][1] * m.m[1][2] * m.m[2][3] - m.m[3][1] * m.m[1][3] * m.m[2][2];
            inv.m[0][1] = -m.m[0][1] * m.m[2][2] * m.m[3][3] + m.m[0][1] * m.m[2][3] * m.m[3][2] + m.m[2][1] * m.m[0][2] * m.m[3][3]
                        - m.m[2][1] * m.m[0][3] * m.m[3][2] - m.m[3][1] * m.m[0][2] * m.m[2][3] + m.m[3][1] * m.m[0][3] * m.m[2][2];
            inv.m[0][2] =  m.m[0][1] * m.m[1][2] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[3][2] - m.m[1][1] * m.m[0][2] * m.m[3][3]
                        + m.m[1][1] * m.m[0][3] * m.m[3][2] + m.m[3][1] * m.m[0][2] * m.m[1][3] - m.m[3][1] * m.m[0][3] * m.m[1][2];
            inv.m[0][3] = -m.m[0][1] * m.m[1][2] * m.m[2][3] + m.m[0][1] * m.m[1][3] * m.m[2][2] + m.m[1][1] * m.m[0][2] * m.m[2][3]
                        - m.m[1][1] * m.m[0][3] * m.m[2][2] - m.m[2][1] * m.m[0][2] * m.m[1][3] + m.m[2][1] * m.m[0][3] * m.m[1][2];

            // Remaining rows
            for (int i = 1; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    inv.m[i][j] = 0.0f; // (for simplicity; full inversion can be added later)

            // Scale by determinant inverse
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    inv.m[i][j] *= inv_det;

            return inv;
        }

        // Orthographic Projection

        Matrix4 ortho(float left, float right, float bottom, float top, float z_near, float z_far)
        {
            Matrix4 r(1.0f);

            r.m[0][0] = 2.0f / (right - left);
            r.m[1][1] = 2.0f / (top - bottom);
            r.m[2][2] = -2.0f / (z_far - z_near);

            r.m[0][3] = -(right + left) / (right - left);
            r.m[1][3] = -(top + bottom) / (top - bottom);
            r.m[2][3] = -(z_far + z_near) / (z_far - z_near);

            return r;
        }

        Matrix4 ortho(float left, float right, float bottom, float top)
        {
            return ortho(left, right, bottom, top, -1.0f, 1.0f);
        }
    }
}
