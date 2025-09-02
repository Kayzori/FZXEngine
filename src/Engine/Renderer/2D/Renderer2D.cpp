#include <Engine/Renderer/2D/Renderer2D.hpp>

bool Renderer2D::isInit() {
    return Initialized;
}

void Renderer2D::Init(int width, int height)
{
    if (Initialized) Delete();
    SCREEN_WIDTH = width;
    SCREEN_HEIGHT = height;
    shaderProgram = GLShaderManager::CreateShaderProgram(fragmentShaderSrc, vertexShaderSrc);

    // Projection: (0,0) at top-left, (width,height) bottom-right
    projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, MAX_VERTICES * sizeof(glm::vec2), nullptr, GL_DYNAMIC_DRAW);

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
    for (Triangle2D t : triangles) {
        if (vertexBatch.size() + 3 >= MAX_VERTICES) break;
        vertexBatch.push_back({t.a, color});
        vertexBatch.push_back({t.b, color});
        vertexBatch.push_back({t.c, color});
    }
}

void Renderer2D::Render()
{
    if (!Initialized) {
        std::cout << "[Failed] Renderer Not Initialized!";
    }
    if (vertexBatch.empty()) return;

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertexBatch.size() * sizeof(Vertex2D), vertexBatch.data());

    glUseProgram(shaderProgram);
    GLuint loc = glGetUniformLocation(shaderProgram, "uProjection");
    glUniformMatrix4fv(loc, 1, GL_FALSE, &projection[0][0]);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertexBatch.size());
    glBindVertexArray(0);

    vertexBatch.clear();
}
