#include "config.h"
#include "Object.h"

int main() {

    GLFWwindow* window = StartGLFW();
    if (!window) return -1;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, 0, screenHeight, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    std::vector<Object> objs = {
        Object({screenWidth / 2.0, screenHeight / 2.0}, {0.0, 0.0}, 30, 5.994e24),
        Object({screenWidth / 2.0, screenHeight / 2.0 + 509}, {1019.8, 0.0}, 8, 7.342e20)
    };

    while(!glfwWindowShouldClose(window)) {

        glClear(GL_COLOR_BUFFER_BIT);

        // Update position
        for(auto& obj : objs) {
            obj.UpdatePos(objs);
        }

        // Draw object
        for(auto& obj : objs) {
            obj.DrawCircle();
        }

        // Close window if ESC key is pressed
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            break; 
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}