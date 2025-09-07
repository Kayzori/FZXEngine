#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Engine/Object/Object.h"
#include "Engine/Servers/PhysicsServer/PhysicsServer.hpp"


std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> distX(100.0f, 1180.0f);
std::uniform_real_distribution<float> distY(100.0f, 620.0f);
std::uniform_real_distribution<float> randi(-1, 1);

std::vector<RigidBody2D*> rigs;
std::unordered_set<RigidBody2D*> inBoard;

// ---------------- Global Variables ----------------

bool windowMoving = false;
std::chrono::high_resolution_clock::time_point lastMoveTime;

// ---------------- GLFW Callbacks ----------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
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

    const int screenWidth = 1280;
    const int screenHeight = 720;
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
    AABB board = {{screenWidth / 2, screenHeight / 2}, {screenWidth / 2, screenHeight / 2}};
    PhysicsServer::CollisionSystem::InitCollisionBoard(board);
    Renderer2D::Init(screenWidth, screenHeight);

    for (int i = 0; i < 100; i++) {
        glm::vec2 randomPos(distX(gen), distY(gen));

        rigs.push_back(new RigidBody2D(
            new Collision2D(
                new Circle2D(25.0f, 16), 
                {0, 0, 0, 0}, 
                {0, 1, 0, 1}, 
                {0, 0, 0, 0}
            ),
            1.0,  // Mass
            1.0f,  // Restitution
            1.0f,  // Friction
            0.0f,  // GravityScale
            0.01f,  // L.Damping
            0.01f   // A.Damping
        ));

        RigidBody2D* newRig = rigs.back();
        newRig->transform->position = randomPos;
        newRig->ApplyForce({640 * randi(gen), 360 * randi(gen)});
    }

    StaticBody2D* sb = new StaticBody2D(new Collision2D(new Box2D(1280, 50), {1, 1, 1, 1}, glm::vec4(0.0f)));
    sb->transform->position = {640, 745};
    
    StaticBody2D* sb2 = new StaticBody2D(new Collision2D(new Box2D(50, 720), {1, 1, 1, 1}, glm::vec4(0.0f)));
    sb2->transform->position = {-25, 360};
    
    StaticBody2D* sb3 = new StaticBody2D(new Collision2D(new Box2D(50, 720), {1, 1, 1, 1}, glm::vec4(0.0f)));
    sb3->transform->position = {1305, 360};

    StaticBody2D* sb4 = new StaticBody2D(new Collision2D(new Box2D(1280, 50), {1, 1, 1, 1}, glm::vec4(0.0f)));
    sb4->transform->position = {640, -25};

    // ---------------- Main Loop ----------------
    while (!glfwWindowShouldClose(window)) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsed = currentTime - lastTime;
        lastTime = currentTime;

        deltaTime += elapsed.count();
        
        while (deltaTime >= FrameTime) {
            
            
            processInput(window);

            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            for (RigidBody2D* rig : rigs) {
                rig->OnUpdate(deltaTime);
                rig->OnDraw();
                if (rig->transform->position.x < 0.0f) rig->transform->position.x = 1280.0f;
                if (rig->transform->position.x > 1280.0f) rig->transform->position.x = 0.0f;
                if (rig->transform->position.y < 0.0f) rig->transform->position.y = 720.0f;
                if (rig->transform->position.y > 720.0f) rig->transform->position.y = 0.0f;
                bool inside = board.contains(rig->transform->position);

                if (inside && inBoard.find(rig) == inBoard.end()) {
                    inBoard.insert(rig);
                } 
                else if (!inside && inBoard.find(rig) != inBoard.end()) {
                    inBoard.erase(rig);
                }
            }

            PhysicsServer::CollisionSystem::UpdateCollisionBoard();
            PhysicsServer::CollisionSystem::RenderCollisionBoard();

            Renderer2D::Render();
            
            std::cout << deltaTime << std::endl;
            std::cout << "rigs : " << rigs.size() << std::endl;
            std::cout << "in board :" << inBoard.size() << std::endl;

            glfwSwapBuffers(window);
            glfwPollEvents();

            deltaTime = 0.0f;
        }
    }

    for(RigidBody2D* rig : rigs) {
        delete rig;
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}