#include "config.hpp"
#include "Object.hpp"
#include "Planets.hpp"

void stampa(int x) {
    std::cout << x << std::endl;
}

int main() {
    GLFWwindow* window = StartGLFW();
    if (!window) return -1;

    glfwSetScrollCallback(window, scroll_callback);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, 0, screenHeight, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    Earth earth;
    Moon moon;

    std::vector<Object> objs = {
        Object( { screenWidth / 2.0, screenHeight / 2.0 }, 
                { 0.0, 0.0},                                    
                int(449840797194900.0 / scaling_factor),
                5.972e24
        ),
        /*Object( { screenWidth / 2.0 + (3.844e8 / scaling_factor), screenHeight / 2.0 }, 
                { 0.0, 1019.8 }, 
                int(480500 / scaling_factor),
                7.348e22
        )
        */
    };
    
    //std::vector<Object> objs = {earth, moon};

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
        glTranslated(-objs[i].getPosX(), -objs[i].getPosY(), 0);

        // Draw objects
        for(auto& obj : objs) {
            obj.DrawCircle();
        }

        // Close window if ESC key is pressed
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            break; 
        }

        if(glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
            i = 0;
        }
        else if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
            i = 1;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}