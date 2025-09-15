#include "config.h"

int screenWidth = 800;   // valori di default
int screenHeight = 600;

GLFWwindow* StartGLFW() {
    if (!glfwInit()) {
        std::cout << "failed to initialize glfw, panic!" << std::endl;
        return nullptr;
    }

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    // aggiorna le variabili globali
    screenWidth = mode->width;
    screenHeight = mode->height;

    // finestra fullscreen sul monitor primario
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Gravity Simulator", monitor, NULL);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    return window;
}