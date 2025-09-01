#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include <Engine/Renderer/GLShaderManager.hpp>
#include <Engine/Renderer/2D/Triangulator2D.hpp>

class Renderer2D {
public:
    Renderer2D(int width, int height);
    ~Renderer2D();

    void DrawShape(const std::vector<glm::vec2>& vertices);
    void Render();

private:
    int screenWidth, screenHeight;
    GLuint VAO, VBO;
    GLuint shaderProgram;
    glm::mat4 projection;
    std::vector<glm::vec2> vertexBatch;

    static const size_t MAX_VERTICES = 10000;

    const char* vertexShaderSrc = R"(
        #version 330 core
        layout(location = 0) in vec2 aPos;
        uniform mat4 uProjection;
        void main() {
            gl_Position = uProjection * vec4(aPos, 0.0, 1.0);
        }
    )";

    const char* fragmentShaderSrc = R"(
        #version 330 core
        out vec4 FragColor;
        void main() {
            FragColor = vec4(0.0, 1.0, 0.0, 1.0);
        }
    )";
};
