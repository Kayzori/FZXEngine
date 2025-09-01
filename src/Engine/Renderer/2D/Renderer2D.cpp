#include <Engine/Renderer/2D/Renderer2D.hpp>

Renderer2D::Renderer2D(int width, int height) :
screenWidth(width),
screenHeight(height)
{
    shaderProgram = GLShaderManager::CreateShaderProgram(fragmentShaderSrc, vertexShaderSrc);

    // Projection: (0,0) at top-left, (width,height) bottom-right
    projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, MAX_VERTICES * sizeof(glm::vec2), nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Renderer2D::~Renderer2D()
{
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);
}

void Renderer2D::DrawShape(const std::vector<glm::vec2>& vertices)
{
    std::vector<Triangle2D> triangles = Triangulator2D::FanTriangulation(vertices);
    for (Triangle2D t : triangles) {
        if (vertexBatch.size() + 3 >= MAX_VERTICES) break;
        vertexBatch.push_back(t.a);
        vertexBatch.push_back(t.b);
        vertexBatch.push_back(t.c);
    }
}

void Renderer2D::Render()
{
    if (vertexBatch.empty()) return;

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertexBatch.size() * sizeof(glm::vec2), vertexBatch.data());

    glUseProgram(shaderProgram);
    GLuint loc = glGetUniformLocation(shaderProgram, "uProjection");
    glUniformMatrix4fv(loc, 1, GL_FALSE, &projection[0][0]);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertexBatch.size());
    glBindVertexArray(0);

    vertexBatch.clear();
}
