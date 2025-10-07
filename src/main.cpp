#include "config.hpp"
#include "Object.hpp"
#include "Planets.hpp"

#include <random>

int main() {

    std::random_device rd; 
    std::mt19937 gen(rd()); 

    double posX;
    double posY;
    double velX;
    double velY;
    double massa;
    int raggio;
    std::vector<int> colors;

    GLFWwindow* window = StartGLFW();
    if (!window) return -1;

    glfwSetScrollCallback(window, scroll_callback);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, 0, screenHeight, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    Earth earth;
    Moon moon;
    
    std::vector<Object> objs = {earth, moon};

    int i = 0;
    bool obj_focus = true;

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

        if(glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
            obj_focus = true;
        }

        else if(glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
            obj_focus = false;
        }

        if(objs.size() > 0 && obj_focus) {
            glTranslated(-objs[i].getPosX(), -objs[i].getPosY(), 0);
        }
        /*
        if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && simulationSpeed <= 8.64e4) {
            simulationSpeed *= 1.1;
        }

        else if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && simulationSpeed > 0) {
            simulationSpeed *= 0.9;
        }
        */

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            break; 
        }

        // Draw objects
        for(auto& obj : objs) {
            if(!obj.getDeleteStatus())
                obj.DrawCircle();
        }

        if(glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
            std::uniform_real_distribution<> posx(-900.0, 900.0);
            std::uniform_real_distribution<> posy(-500.0, 500.0);
            std::uniform_real_distribution<> vel(0.0, 300.0);
            std::uniform_real_distribution<> mass(7e22, 7e25);
            std::uniform_int_distribution<> radius(1, 40);
            std::uniform_int_distribution<> color(150, 255);
            posX = posx(rd);
            posY = posy(rd);
            velX = vel(rd);
            velY = vel(rd);
            massa = mass(rd);
            raggio = radius(rd);
            colors = {color(rd), color(rd), color(rd)};

            objs.push_back(Object({posX, posY}, {velX, velY}, raggio, massa, colors));
        }        

        bool erase = false;

        for(auto& obj : objs) {
            obj.check_should_delete(objs);
            erase = true;
        }
        if(erase) {
            for(int idx = 0; idx < objs.size(); idx++) {
                if(objs[idx].getDeleteStatus()) {
                    objs.erase(objs.begin() + idx);
                }
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