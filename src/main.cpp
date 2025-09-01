#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>

#include <Engine/Renderer/2D/Renderer2D.hpp>
#include <Engine/Components/Transform2D.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const int screenWidth = 800;
    const int screenHeight = 600;
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Renderer2D", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    glViewport(0, 0, screenWidth, screenHeight);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    auto lastTime = std::chrono::high_resolution_clock::now();
    
    const float FPS = 60.0f;
    const float FrameTime = 1.0f / FPS;
    
    // Init --------------
    Renderer2D renderer(screenWidth, screenHeight);

    std::vector<glm::vec2> square = {
        {-10.0f, 10.0f },
        {10.0f, 10.0f},
        {10.0f, -10.0f},
        {-10.0f, -10.0f}
    };

    Transform2D squareTransform;
    squareTransform.position = {screenWidth/4.0f, screenHeight/2.0f};
    float deltaTime = 0.0f;
    static bool scalingUp = true;
    // --------------------------

    while (!glfwWindowShouldClose(window)) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsed = currentTime - lastTime;
        lastTime = currentTime;
        deltaTime += elapsed.count();

        while (deltaTime >= FrameTime) {
            // Updating --------
            processInput(window);

            if (scalingUp) {
                squareTransform.scale += 5.0f * deltaTime;
                squareTransform.position.x += 150.0f * deltaTime;
                squareTransform.rotation += 50.0f *deltaTime;
                if (squareTransform.scale.x >= 10.0f)
                    scalingUp = false;
            } else {
                squareTransform.scale -= 5.0f * deltaTime;
                squareTransform.position.x -= 150.0f * deltaTime;
                squareTransform.rotation -= 50.0f *deltaTime;
                if (squareTransform.scale.x <= 1.0f)
                    scalingUp = true;
            }

            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            std::vector<glm::vec2> transformedSquare = square;
            squareTransform.Apply(transformedSquare);
            renderer.DrawShape(transformedSquare);
            renderer.Render();

            glfwSwapBuffers(window);
            glfwPollEvents();
            // ------------------
            deltaTime -= FrameTime;
        }
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
