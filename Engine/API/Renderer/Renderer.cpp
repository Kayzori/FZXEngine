#include "Renderer.h"

#include "GLShaderManager.h"
#include "Math/Algebra/Matrix/Utils.h"
#include "Math/Algebra/Vector/Utils.h"

#include <limits>
#include <algorithm>

static const size_t RENDERER_MAX_VERTICES = 65536;

// Constructors and Destructors
Renderer::Renderer(int width, int height) {
    shader_program = GLShaderManager::createShaderProgram(fragment_shader_src, vertex_shader_src);

    projection = Math::Algebra::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(RENDERER_MAX_VERTICES * sizeof(RenderVertex2D)), nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, (GLsizei)sizeof(RenderVertex2D), (void*)offsetof(RenderVertex2D, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, (GLsizei)sizeof(RenderVertex2D), (void*)offsetof(RenderVertex2D, color));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    vertex_batch.reserve(RENDERER_MAX_VERTICES);
}

Renderer::~Renderer() {
    if (vbo) glDeleteBuffers(1, &vbo);
    if (vao) glDeleteVertexArrays(1, &vao);
    if (shader_program) {
        glUseProgram(0);
        glDeleteProgram(shader_program);
    }

    vbo = 0;
    vao = 0;
    shader_program = 0;
}

// Methods

void Renderer::drawPolygonArea(const std::vector<Vector2>& verts, const Vector4& color) {
    if (verts.size() < 3) return;

    for (size_t i = 1; i + 1 < verts.size(); i++) {
        vertex_batch.push_back({ verts[0], color });
        vertex_batch.push_back({ verts[i], color });
        vertex_batch.push_back({ verts[i + 1], color });
    }
}

void Renderer::drawPolygonSegments(const std::vector<Vector2>& verts, const Vector4& color, float thickness) {
    if (verts.size() < 2 || thickness <= 0) return;

    // Draw each consecutive segment and close the loop (last -> first)
    for (size_t i = 0; i < verts.size(); i++) {
        Vector2 p0 = verts[i];
        Vector2 p1 = verts[(i + 1) % verts.size()];
        Vector2 dir = p1 - p0;
        float length = Math::Algebra::length(dir);
        if (length < 0.001f) continue;

        dir = Math::Algebra::normalize(dir);
        Vector2 normal(-dir.y, dir.x);
        normal *= (thickness / 2.0f);

        Vector2 v0 = p0 + normal;
        Vector2 v1 = p1 + normal;
        Vector2 v2 = p1 - normal;
        Vector2 v3 = p0 - normal;

        vertex_batch.push_back({ v0, color });
        vertex_batch.push_back({ v1, color });
        vertex_batch.push_back({ v2, color });
        vertex_batch.push_back({ v2, color });
        vertex_batch.push_back({ v3, color });
        vertex_batch.push_back({ v0, color });
    }
}

void Renderer::drawPolygonVertices(const std::vector<Vector2>& verts, const Vector4& color, float size) {
    if (size <= 0) return;

    float half = size / 2;
    for (const auto& p : verts) {
        Vector2 v0 = p + Vector2(-half, -half);
        Vector2 v1 = p + Vector2(half, -half);
        Vector2 v2 = p + Vector2(half, half);
        Vector2 v3 = p + Vector2(-half, half);

        vertex_batch.push_back({ v0, color });
        vertex_batch.push_back({ v1, color });
        vertex_batch.push_back({ v2, color });
        vertex_batch.push_back({ v2, color });
        vertex_batch.push_back({ v3, color });
        vertex_batch.push_back({ v0, color });
    }
}

void Renderer::render() {
    glUseProgram(shader_program);
    GLuint loc = glGetUniformLocation(shader_program, "uProjection");
    if (loc != -1) {
        glUniformMatrix4fv(loc, 1, GL_TRUE, &projection.m[0][0]);
    }

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Draw in chunks if the batch exceeds the GPU buffer capacity
    size_t totalVerts = vertex_batch.size();
    size_t offsetVerts = 0;
    while (offsetVerts < totalVerts) {
        size_t chunk = std::min(RENDERER_MAX_VERTICES, totalVerts - offsetVerts);
        glBufferSubData(GL_ARRAY_BUFFER, 0, (GLsizeiptr)(chunk * sizeof(RenderVertex2D)), vertex_batch.data() + offsetVerts);
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)chunk);
        offsetVerts += chunk;
    }

    vertex_batch.clear();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}
