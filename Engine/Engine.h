#pragma once

// Engine core header - provides a simple engine that owns the GLFW window,
// a Renderer instance and manages a list of Node2D objects to update and
// render each frame.

// OpenGL (load glad here so other headers don't re-include it)
#include <glad/glad.h>
#define GLAD_INCLUDED
#include <GLFW/glfw3.h>

// Math
#define MATH_INCLUDE_ALGEBRA
#define MATH_INCLUDE_VECTOR_UTILS
#define MATH_INCLUDE_MATRIX_UTILS
#define MATH_INCLUDE_GEOMETRY2D
#define MATH_INCLUDE_GEOMETRY2D_UTILS
#define MATH_INCLUDE_TRIGONOMETRY_UTILS
#include "Math/Math.h"

// Custom API
#include "API/Renderer/Renderer.h"

// Engine objects
#include "2D/2D.h"

#include <vector>
#include <string>

class Engine {
public:
	Engine(int width = 800, int height = 600, const std::string& title = "FZXEngine");
	~Engine();

	bool init();

	void run();

	void stop();

	void addNode(Node* node);
	void removeNode(Node* node);

	GLFWwindow* getWindow() const { return window; }
	Renderer* getRenderer() const { return renderer; }

private:
	void update(float dt);
	void render();

	static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

	std::vector<Node*> nodes;
	Renderer* renderer = nullptr;
	GLFWwindow* window = nullptr;

	int width;
	int height;
	std::string title;

	bool running = false;
};
