#include "config.hpp"
#include "Object.hpp"
#include "Planets.hpp"

int main() {
    GLFWwindow* window = StartGLFW();
    if (!window) return -1;

    glfwSetScrollCallback(window, scroll_callback);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, 0, screenHeight, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    std::vector<Object> objs = {
        Object( { screenWidth / 2.0, screenHeight / 2.0 }, 
                { 0.0, 0.0},                                    
                int(12745594.0 / scaling_factor),
                5.972e24
        ),
        Object( { screenWidth / 2.0 + (3.844e8 / scaling_factor), screenHeight / 2.0 }, 
                { 0.0, 1019.8 }, 
                int(3474800.0 / scaling_factor),
                7.348e22
        )
    };

    int i = 0;

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();

        // Update position
        for(auto& obj : objs) {
            obj.UpdatePos(objs);
        }

        glLoadIdentity();
        glTranslated(screenWidth/2.0, screenHeight/2.0, 0);
        glScaled(zoom, zoom, 1.0);
        glTranslated(-objs[0].position[0], -objs[0].position[1], 0);

        // Draw objects
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