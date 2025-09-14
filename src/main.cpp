#include "config.h"

GLFWwindow* StartGLFW();

int main() {

    GLFWwindow* window = StartGLFW();
    if (!window) return -1;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, 0, screenHeight, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    double lastTime = glfwGetTime();

    std::vector<Object> objs = {
        Object({screenWidth / 2.0f, screenHeight / 2.0f}, {0.0f, 0.0f}, 30, 700),
        Object({screenWidth / 2.0f, 1049.0f}, {0.0f, 0.0f}, 8, 700)
    };

    while(!glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        updateDeltaTime(objs, deltaTime);
        drawObjects(objs);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

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