#include "config.h"
#include "Object.h"

int main() {

    GLFWwindow* window = StartGLFW();
    if (!window) return -1;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, 0, screenHeight, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    double lastTime = glfwGetTime();

    std::vector<Object> objs = {
        Object({screenWidth / 2.0, screenHeight / 2.0}, {0.0, 0.0}, 30, 5.994e24),
        Object({screenWidth / 2.0, 750.0}, {1587.955f, 0.0}, 8, 7.342e22)
    };

    /*for(auto& obj : objs) {
        obj.CalculatePullFactor(objs);
    }
    */

    while(!glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        glClear(GL_COLOR_BUFFER_BIT);

        // Update deltaTime
        for(auto& obj : objs) {
            obj.getDeltaTime(deltaTime);
        }

        // Calculate pulling factor
        for(auto& obj : objs) {
            obj.CalculatePullFactor(objs);
        }

        // Update position
        for(auto& obj : objs) {
            obj.UpdatePos();
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