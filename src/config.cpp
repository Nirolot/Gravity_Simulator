#include "config.hpp"

int screenWidth = 800;
int screenHeight = 600;
double zoom = 1.0;
double simulationSpeed = 8.64e2;

GLFWwindow* StartGLFW() {
    if (!glfwInit()) {
        std::cout << "failed to initialize glfw, panic!" << std::endl;
        return nullptr;
    }

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    screenWidth = mode->width;
    screenHeight = mode->height;

    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Gravity Simulator", monitor, NULL);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    return window;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    if (yoffset < 0) {
        zoom *= 0.9;  
    } else if (yoffset > 0) {
        zoom *= 1.1;
    }

    if (zoom < 0.01) zoom = 0.01; 
    if (zoom > 10000.0) zoom = 10000.0; 
}
