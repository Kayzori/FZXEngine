#include <glad/glad.h>
#include <GLFW/glfw3.h>

/*
camleCase: Functions/Methods
PascalCase: Classes/Structures/Namespaces
snake_case: Variables/Properties

*/

int main() {
    GLFWwindow* window;
    glfwInit();
    window = glfwCreateWindow(800, 600, "OpenGL Project", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    while (!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}