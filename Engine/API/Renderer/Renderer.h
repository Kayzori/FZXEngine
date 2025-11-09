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
    void drawPolygonArea(const std::vector<Vector2>& verts, const Vector4& color);
    void drawPolygonSegments(const std::vector<Vector2>& verts, const Vector4& color, float thickness = 1.0f);
    void drawPolygonVertices(const std::vector<Vector2>& verts, const Vector4& color, float size = 1.0f);
    void render();

private:
    // Properties
    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint shader_program = 0;

    Matrix4 projection;

    std::vector<RenderVertex2D> vertex_batch;

    const char* vertex_shader_src = R"(
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

    const char* fragment_shader_src = R"(
        #version 330 core
        in vec4 fColor;
        out vec4 FragColor;

        void main() {
            FragColor = fColor;
        }
    )";
};
