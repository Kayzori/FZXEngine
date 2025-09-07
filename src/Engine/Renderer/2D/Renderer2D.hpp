#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "../GLShaderManager.hpp"
#include "Triangulator2D.hpp"

struct Vertex2D {
    glm::vec2 position;
    glm::vec4 color;
};

class Renderer2D {
public:
    static bool isInit();
    static void Init(int width, int height);
    static void Delete();

    static void DrawPolygon(const std::vector<glm::vec2>& vertices, const glm::vec4& color);
    static void DrawLines(const std::vector<glm::vec2>& points, const glm::vec4& color, float thickness = 1.0f);
    static void DrawPoints(const std::vector<glm::vec2>& points, const glm::vec4& color, float size = 1.0f);

    static void Render();

private:
    static inline bool Initialized = false;
    static inline int SCREEN_WIDTH = 800;
    static inline int SCREEN_HEIGHT = 600;

    static inline GLuint VAO = 0;
    static inline GLuint VBO = 0;
    static inline GLuint shaderProgram = 0;

    static inline glm::mat4 projection;

    static constexpr size_t MAX_VERTICES = 1000000;
    static inline std::vector<Vertex2D> vertexBatch;

    static inline const char* vertexShaderSrc = R"(
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

    static inline const char* fragmentShaderSrc = R"(
        #version 330 core
        in vec4 fColor;
        out vec4 FragColor;
        void main() {
            FragColor = fColor;
        }
    )";
};
