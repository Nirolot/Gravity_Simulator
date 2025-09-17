#include "config.h"

int screenWidth = 800;   
int screenHeight = 600;
double zoom = 1.0; 

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

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    if (yoffset < 0) {
        zoom *= 0.9;  // zoom in (riduci)
    } else if (yoffset > 0) {
        zoom *= 1.1;  // zoom out (aumenti)
    }

    if (zoom < 0.1) zoom = 0.1; // limite minimo
    if (zoom > 10.0) zoom = 10.0; // limite massimo
}
