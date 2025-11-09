#include "Engine.h"

#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>

Engine::Engine(int width, int height, const std::string& title)
    : width(width), height(height), title(title) {}

Engine::~Engine() {
    if (renderer) {
        delete renderer;
        renderer = nullptr;
    }
    if (window) {
        glfwDestroyWindow(window);
        window = nullptr;
    }
    glfwTerminate();
}

bool Engine::init() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return false;
    }

    // Request OpenGL 3.3 Core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // Load glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        glfwDestroyWindow(window);
        window = nullptr;
        glfwTerminate();
        return false;
    }

    // Create renderer
    renderer = new Renderer(width, height);
    return true;
}

void Engine::run() {
    if (!window || !renderer) return;

    running = true;

    using clock = std::chrono::high_resolution_clock;
    auto last = clock::now();

    const std::chrono::duration<float> targetFrameTime(1.0 / 60.0);

    while (running && !glfwWindowShouldClose(window)) {
        auto frameStart = clock::now();

        std::chrono::duration<float> dtDuration = frameStart - last;
        last = frameStart;
        float dt = dtDuration.count();

        glfwPollEvents();

        update(dt);
        render();

        glfwSwapBuffers(window);

        auto frameEnd = clock::now();
        std::chrono::duration<float> frameTime = frameEnd - frameStart;
        if (frameTime < targetFrameTime) {
            auto sleepTime = std::chrono::duration_cast<std::chrono::microseconds>(targetFrameTime - frameTime);
            std::this_thread::sleep_for(sleepTime);
            last = clock::now();
        }
    }
}

void Engine::stop() {
    running = false;
}

void Engine::addNode(Node* node) {
    if (!node) return;
    nodes.push_back(node);
}

void Engine::removeNode(Node* node) {
    if (!node) return;
    auto it = std::find(nodes.begin(), nodes.end(), node);
    if (it != nodes.end()) nodes.erase(it);
}

void Engine::update(float dt) {
    for (auto node : nodes) {
        if (node) node->onUpdate(dt);
    }
}

void Engine::render() {
    // Clear with dark gray background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Render each node
    for (auto node : nodes) {
        if (node) node->onRender(renderer);
    }

    if (renderer) renderer->render();
}

void Engine::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    (void)window;
    glViewport(0, 0, width, height);
}
