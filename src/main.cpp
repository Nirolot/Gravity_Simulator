#include "config.hpp"
#include "Object.hpp"
#include "Planets.hpp"

#include <random>

int main() {

    std::random_device rd; // Seed generator
    std::mt19937 gen(rd()); // Mersenne Twister engine

    GLFWwindow* window = StartGLFW();
    if (!window) return -1;

    glfwSetScrollCallback(window, scroll_callback);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, 0, screenHeight, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    Earth earth;
    Moon moon;
    
    std::vector<Object> objs = {earth};

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
            if(!obj.getDeleteStatus())
                obj.DrawCircle();
        }

        // Close window if ESC key is pressed
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            break; 
        }

        if(glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
            std::uniform_real_distribution<> posx(-900.0, 900.0);
            std::uniform_real_distribution<> posy(-500.0, 500.0);
            std::uniform_real_distribution<> vel(0.0, 300.0);
            std::uniform_real_distribution<> mass(7e22, 7e25);
            std::uniform_int_distribution<> radius(1, 40);
            std::uniform_int_distribution<> color(150, 255);
            double posX = posx(rd);
            double posY = posy(rd);
            double velX = vel(rd);
            double velY = vel(rd);
            double massa = mass(rd);
            int raggio = radius(rd);
            std::vector<int> colors = {color(rd), color(rd), color(rd)};

            objs.push_back(Object({posX, posY}, {velX, velY}, raggio, massa, colors));
        }        

        for(auto& obj : objs) {
            obj.check_should_delete(objs);
        }

        for(int idx = 0; idx < objs.size(); idx++) {
            if(objs[idx].getDeleteStatus()) {
                objs.erase(objs.begin() + idx);
            }
        }  


        if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && i < objs.size() - 1) {
            i++;
        } 
        if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && i > 0) {
            i--;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
        
        
    }

    glfwTerminate();
    return 0;
}