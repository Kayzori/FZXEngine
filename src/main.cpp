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
    const float FPS = 120.0f;
    const float FrameTime = 1.0f / FPS;
    float deltaTime = 0.0f;

    // ---------------- Init Scene ----------------
    Physics2DServer::CollisionSystem::InitCollisionBoard({{0, 0}, {screenWidth / 2, screenHeight / 2}});
    Renderer2D::Init(screenWidth, screenHeight);

    RigidBody2D* rb = new RigidBody2D(
        new Collision2D(new Box2D(50.0f), {0, 1, 0, 1}),
        1.0f,   // Mass
        1.0f,   // Restitution
        0.0f    // Friction
    );
    rb->ApplyForce(glm::vec2(10000.0f * 10.0f, -10000.0f));
    rb->transform->position = {200, 100};

    RigidBody2D* rb2 = new RigidBody2D(
        new Collision2D(new Circle2D(50.0f, 16), {0, 1, 0, 1}),
        1.0f,   // Mass
        1.0f,   // Restitution
        0.0f    // Friction
    );
    rb2->ApplyForce(glm::vec2(-10000.0f * 10.0f, -10000.0f));
    rb2->transform->position = {600, 100};

    RigidBody2D* rb4 = new RigidBody2D(
        new Collision2D(new Circle2D(20.0f, 16), {0, 1, 0, 1}),
        1.0f,   // Mass
        1.0f,   // Restitution
        0.0f    // Friction
    );
    rb4->transform->position = {100, 100};

    StaticBody2D* sb = new StaticBody2D(new Collision2D(new Box2D(800, 50), {1, 1, 1, 1}, glm::vec4(0.0f)));
    sb->transform->position = {400, 625};
    
    StaticBody2D* sb2 = new StaticBody2D(new Collision2D(new Box2D(50, 600), {1, 1, 1, 1}, glm::vec4(0.0f)));
    sb2->transform->position = {-25, 300};
    
    StaticBody2D* sb3 = new StaticBody2D(new Collision2D(new Box2D(50, 600), {1, 1, 1, 1}, glm::vec4(0.0f)));
    sb3->transform->position = {825, 300};

    StaticBody2D* sb4 = new StaticBody2D(new Collision2D(new Box2D(800, 50), {1, 1, 1, 1}, glm::vec4(0.0f)));
    sb4->transform->position = {400, -25};

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

            glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            
            rb4->OnUpdate(FrameTime);   
            rb2->OnUpdate(FrameTime);
            rb->OnUpdate(FrameTime);
            rb->OnDraw();
            rb2->OnDraw();
            rb4->OnDraw();
            
            
            
            sb->OnDraw();
            sb2->OnDraw();
            sb3->OnDraw();
            sb4->OnDraw();

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
