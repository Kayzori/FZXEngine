#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <chrono>

#include <Engine/Renderer/2D/Renderer2D.hpp>
#include <Engine/Component/Component.h>
#include <Engine/Object/Object.h>
#include <Engine/Servers/PhysicsServer/PhysicsServer.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

glm::vec2 direction = {0.0f, 0.0f};
float rotation = 0.0f;
float speed = 100.0;

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
    PhysicsServer::Init({0, 0}, {screenWidth/2, screenHeight/2});
    Renderer2D::Init(screenWidth, screenHeight);
    Collision2D* col = new Collision2D(new Rect2D(100.0f, 100.0f));
    Collision2D* col2 = new Collision2D(new Circle2D(100.0f, 32), glm::vec4(1, 0, 0, 1));
    Collision2D* col3 = new Collision2D(new Circle2D(25.0f, 5), glm::vec4(0, 0, 1, 1));
    col->transform->offset = {300.0, 300.0};
    col2->transform->offset = {249.0, 300.0};
    col3->transform->offset = {600.0, 450.0};
    

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

            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            col->transform->position += direction * speed * deltaTime;
            col->transform->rotation += rotation * deltaTime;
            std::cout << "While Is Colliding: " << (col->isColliding() ? "true" : "false") << std::endl;
            col->OnDraw();
            col2->OnDraw();
            col3-> OnDraw();
            Renderer2D::Render();

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
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        direction = {0, -1};
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        direction = {0, 1};
    else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        direction = {-1, 0};
    else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        direction = {1, 0};
    else direction = {0, 0};
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        rotation = 50.0f;
    else rotation = 0.0f;
}
