#pragma once
#include <glad/glad.h>
#include <Engine/Renderer/GLShaderManager.hpp>
#include <Engine/Renderer/2D/Triangulator2D.hpp>

struct Vertex2D {
    glm::vec2 position;
    glm::vec4 color;
};

class Renderer2D {
public:
    static void Init(int width, int height);
    static void Delete();
    static bool isInit();

    static void DrawPolygon(const std::vector<glm::vec2> vertices, const glm::vec4 color = glm::vec4(1,1,1,1));
    static void Render();

private:
    // Flags
    inline static bool Initialized = false;
    // ScreenSize
    inline static int SCREEN_WIDTH;
    inline static int SCREEN_HEIGHT;

    inline static GLuint VAO, VBO;
    inline static GLuint shaderProgram;
    inline static glm::mat4 projection;
    inline static std::vector<Vertex2D> vertexBatch;
    inline static const size_t MAX_VERTICES = 10000;

    inline static const char* vertexShaderSrc = R"(
        #version 330 core
        layout(location = 0) in vec2 aPos;
        layout(location = 1) in vec4 aColor;
        out vec4 vColor;
        uniform mat4 uProjection;
        void main() {
            gl_Position = uProjection * vec4(aPos, 0.0, 1.0);
            vColor = aColor;
        }
    )";

    inline static const char* fragmentShaderSrc = R"(
        #version 330 core
        in vec4 vColor;
        out vec4 FragColor;
        void main() {
            FragColor = vColor;
        }
    )";
};
