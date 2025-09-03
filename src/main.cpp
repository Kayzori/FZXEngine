#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <chrono>

#include "Engine/Renderer/2D/Renderer2D.hpp"
#include "Engine/Component/Components.h"
#include "Engine/Object/Object.h"
#include "Engine/Servers/PhysicsServer/PhysicsServer.hpp"

// ---------------- Global Variables ----------------
glm::vec2 direction = {0.0f, 0.0f};
float rotation = 0.0f;
float speed = 100.0f;

bool windowMoving = false;
std::chrono::high_resolution_clock::time_point lastMoveTime;

// ---------------- GLFW Callbacks ----------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void window_position_callback(GLFWwindow* window, int xpos, int ypos) {
    windowMoving = true;
    lastMoveTime = std::chrono::high_resolution_clock::now();
}

// ---------------- Input ----------------
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
    else
        direction = {0, 0};

    rotation = (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) ? 50.0f : 0.0f;
}

// ---------------- Main ----------------
int main() {
    // Initialize GLFW
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

    // Setup callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetWindowPosCallback(window, window_position_callback);

    glViewport(0, 0, screenWidth, screenHeight);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Initialize timing
    auto lastTime = std::chrono::high_resolution_clock::now();
    lastMoveTime = lastTime;
    const float FPS = 60.0f;
    const float FrameTime = 1.0f / FPS;
    float deltaTime = 0.0f;

    // ---------------- Init Scene ----------------
    Physics2DServer::CollisionSystem::InitCollisionBoard({{0, 0}, {screenWidth / 2, screenHeight / 2}});
    Renderer2D::Init(screenWidth, screenHeight);

    RigidBody2D* rb = new RigidBody2D(
        new Collision2D(new Circle2D(25.0f, 16), {1, 0, 0, 1}),
        1.0f,
        false
    );
    rb->transform->position = {300, 0};

    RigidBody2D* rb3 = new RigidBody2D(
        new Collision2D(new Circle2D(25.0f, 16), {1, 0, 0, 1}),
        1.0f,
        false
    );
    rb3->transform->position = {350, -200};

    RigidBody2D* rb5 = new RigidBody2D(
        new Collision2D(new Circle2D(25.0f, 16), {1, 0, 0, 1}),
        1.0f,
        false
    );
    rb5->transform->position = {400, -400};

    RigidBody2D* rb7 = new RigidBody2D(
        new Collision2D(new Circle2D(25.0f, 16), {1, 0, 0, 1}),
        1.0f,
        false
    );
    rb7->transform->position = {450, -600};

    RigidBody2D* rb9 = new RigidBody2D(
        new Collision2D(new Circle2D(25.0f, 16), {1, 0, 0, 1}),
        1.0f,
        false
    );
    rb9->transform->position = {475, -800};

    StaticBody2D* sb = new StaticBody2D(new Collision2D(new Box2D(800, 50), {1, 1, 0, 1}));
    sb->transform->position = {400, 450};


    // ---------------- Main Loop ----------------
    while (!glfwWindowShouldClose(window)) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsed = currentTime - lastTime;
        lastTime = currentTime;

        auto sinceMove = std::chrono::duration<float>(currentTime - lastMoveTime).count();
        if (windowMoving && sinceMove > 0.05f)
            windowMoving = false;

        if (windowMoving) {
            glfwPollEvents();
            continue;
        }

        deltaTime += elapsed.count();
        
        while (deltaTime >= FrameTime) {
            
            processInput(window);

            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            
            rb->OnUpdate(FrameTime);
            rb3->OnUpdate(FrameTime);
            rb5->OnUpdate(FrameTime);
            rb7->OnUpdate(FrameTime);
            rb9->OnUpdate(FrameTime);
            rb->OnDraw();
            rb3->OnDraw();
            rb5->OnDraw();
            rb7->OnDraw();
            rb9->OnDraw();
            sb->OnDraw();

            Renderer2D::Render();

            glfwSwapBuffers(window);
            glfwPollEvents();

            deltaTime -= FrameTime;
        }
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
