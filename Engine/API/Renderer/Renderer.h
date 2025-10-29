#pragma once

// 29-10-2025 :
// For now Renderer is limited for only convex shapes polygon (Triangulation algorithm missing)

#include "Math/Algebra/Vector/Vector2.h"
#include "Math/Algebra/Vector/Vector4.h"
#include "Math/Algebra/Matrix/Matrix4.h"

#ifndef GLAD_INCLUDED
#include <glad/glad.h>
#endif

#include <vector>
#include <string>

struct RenderVertex2D {
    Vector2 position;
    Vector4 color;
};

class Renderer {
public:
    // Constructors and Destructors
    Renderer(int width, int height);
    ~Renderer();

    // Methods
    void DrawPolygon(const std::vector<Vector2>& verts, const Vector4& color);
    void DrawPolygonLines(const std::vector<Vector2>& verts, const Vector4& color, float thickness = 1.0f);
    void DrawPolygonVerts(const std::vector<Vector2>& verts, const Vector4& color, float size = 1.0f);
    void Render();

private:
    // Properties
    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint shaderProgram = 0;

    Matrix4 projection;

    std::vector<RenderVertex2D> vertexBatch;

    const char* vertexShaderSrc = R"(
        #version 330 core
        layout(location = 0) in vec2 aPos;
        layout(location = 1) in vec4 aColor;

        uniform mat4 uProjection;

        out vec4 fColor;

        void main() {
            fColor = aColor;
            gl_Position = uProjection * vec4(aPos, 0.0, 1.0);
        }
    )";

    const char* fragmentShaderSrc = R"(
        #version 330 core
        in vec4 fColor;
        out vec4 FragColor;

        void main() {
            FragColor = fColor;
        }
    )";
};
