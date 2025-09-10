#include "Renderer2D.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

bool Renderer2D::isInit() { return Initialized; }

void Renderer2D::Init(int width, int height) {
    if (Initialized) Delete();

    SCREEN_WIDTH = width;
    SCREEN_HEIGHT = height;

    shaderProgram = GLShaderManager::CreateShaderProgram(fragmentShaderSrc, vertexShaderSrc);
    projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, MAX_VERTICES * sizeof(Vertex2D), nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)offsetof(Vertex2D, color));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Initialized = true;
}

void Renderer2D::Delete() {
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);
    Initialized = false;
}

void Renderer2D::DrawPolygon(const std::vector<glm::vec2>& vertices, const glm::vec4& color) {
    std::vector<Triangle2D> triangles = Triangulator2D::FanTriangulation(vertices);
    for (auto& t : triangles) {
        if (vertexBatch.size() + 3 >= MAX_VERTICES) break;
        vertexBatch.push_back({t.a, color});
        vertexBatch.push_back({t.b, color});
        vertexBatch.push_back({t.c, color});
    }
}

void Renderer2D::DrawLines(const std::vector<glm::vec2>& points, const glm::vec4& color, float thickness) {
    size_t n = points.size();
    if (n < 2) return;

    for (size_t i = 0; i < n; i++) {
        glm::vec2 p0 = points[i];
        glm::vec2 p1 = points[(i + 1) % n];

        glm::vec2 dir = glm::normalize(p1 - p0);
        glm::vec2 normal = glm::vec2(-dir.y, dir.x) * (thickness / 2.0f);

        // Quad vertices
        glm::vec2 v0 = p0 + normal;
        glm::vec2 v1 = p1 + normal;
        glm::vec2 v2 = p1 - normal;
        glm::vec2 v3 = p0 - normal;

        if (vertexBatch.size() + 6 >= MAX_VERTICES) break;
        // Two triangles
        vertexBatch.push_back({v0, color});
        vertexBatch.push_back({v1, color});
        vertexBatch.push_back({v2, color});
        vertexBatch.push_back({v2, color});
        vertexBatch.push_back({v3, color});
        vertexBatch.push_back({v0, color});
    }
}

void Renderer2D::DrawPoints(const std::vector<glm::vec2>& points, const glm::vec4& color, float size) {
    float half = size / 2.0f;

    for (auto& p : points) {
        if (vertexBatch.size() + 6 >= MAX_VERTICES) break;

        glm::vec2 v0 = p + glm::vec2(-half, -half);
        glm::vec2 v1 = p + glm::vec2(half, -half);
        glm::vec2 v2 = p + glm::vec2(half, half);
        glm::vec2 v3 = p + glm::vec2(-half, half);

        // two triangles
        vertexBatch.push_back({v0, color});
        vertexBatch.push_back({v1, color});
        vertexBatch.push_back({v2, color});
        vertexBatch.push_back({v2, color});
        vertexBatch.push_back({v3, color});
        vertexBatch.push_back({v0, color});
    }
}

void Renderer2D::Render() {
    if (!Initialized) {
        std::cout << "[Failed] Renderer Not Initialized!\n";
        return;
    }

    if (vertexBatch.empty()) return;

    glUseProgram(shaderProgram);
    GLuint loc = glGetUniformLocation(shaderProgram, "uProjection");
    glUniformMatrix4fv(loc, 1, GL_FALSE, &projection[0][0]);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferSubData(GL_ARRAY_BUFFER, 0, vertexBatch.size() * sizeof(Vertex2D), vertexBatch.data());
    glDrawArrays(GL_TRIANGLES, 0, vertexBatch.size());

    vertexBatch.clear();
    glBindVertexArray(0);
}
