#include "Renderer2D.hpp"
#include <iostream>

bool Renderer2D::isInit() {
    return Initialized;
}

void Renderer2D::Init(int width, int height)
{
    if (Initialized) Delete();
    SCREEN_WIDTH = width;
    SCREEN_HEIGHT = height;

    shaderProgram = GLShaderManager::CreateShaderProgram(fragmentShaderSrc, vertexShaderSrc);

    // Top-left origin
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

void Renderer2D::Delete()
{
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);
    Initialized = false;
}

void Renderer2D::DrawPolygon(const std::vector<glm::vec2> vertices, const glm::vec4 color)
{
    std::vector<Triangle2D> triangles = Triangulator2D::FanTriangulation(vertices);
    for (auto& t : triangles) {
        if (triangleBatch.size() + 3 >= MAX_VERTICES) break;
        triangleBatch.push_back({t.a, color});
        triangleBatch.push_back({t.b, color});
        triangleBatch.push_back({t.c, color});
    }
}

void Renderer2D::DrawLines(const std::vector<glm::vec2> points, const glm::vec4 color)
{
    size_t n = points.size();
    if (n < 2) return;

    for (size_t i = 0; i < n; i++) {
        if (lineBatch.size() + 2 >= MAX_VERTICES) break;
        lineBatch.push_back({points[i], color});
        lineBatch.push_back({points[(i + 1) % n], color});
    }
}

void Renderer2D::Render()
{
    if (!Initialized) {
        std::cout << "[Failed] Renderer Not Initialized!\n";
        return;
    }

    glUseProgram(shaderProgram);
    GLuint loc = glGetUniformLocation(shaderProgram, "uProjection");
    glUniformMatrix4fv(loc, 1, GL_FALSE, &projection[0][0]);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Render triangles
    if (!triangleBatch.empty()) {
        glBufferSubData(GL_ARRAY_BUFFER, 0, triangleBatch.size() * sizeof(Vertex2D), triangleBatch.data());
        glDrawArrays(GL_TRIANGLES, 0, triangleBatch.size());
        triangleBatch.clear();
    }

    // Render lines
    if (!lineBatch.empty()) {
        glBufferSubData(GL_ARRAY_BUFFER, 0, lineBatch.size() * sizeof(Vertex2D), lineBatch.data());
        glDrawArrays(GL_LINES, 0, lineBatch.size());
        lineBatch.clear();
    }

    glBindVertexArray(0);
}
