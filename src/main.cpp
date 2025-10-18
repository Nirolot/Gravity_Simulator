#include "config.hpp"
#include "Object.hpp"
#include "Planets.hpp"

#include <random>
#include <chrono>

const int maxSimSpeed = 80;

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

    Sun sun;
    Mercury mercury;
    Venus venus;
    Earth earth;
    Moon moon;
    Mars mars;
    Jupiter jupiter;
    Saturn saturn;
    Uranus uranus;
    Neptune neptune;

    std::vector<Object> objs = {sun, mercury, venus, earth, moon, mars, jupiter, saturn, uranus, neptune};

    int i = 0, simSpeed = 1;
    bool obj_focus = true;

    auto start = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();

    while (!glfwWindowShouldClose(window)) {

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        // Campo visivo in base allo zoom
        double viewWidth = screenWidth / zoom;
        double viewHeight = screenHeight / zoom;
        glOrtho(-viewWidth / 2.0, viewWidth / 2.0, -viewHeight / 2.0, viewHeight / 2.0, -1, 1);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glScaled(zoom, zoom, 1.0);

        if(glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
            obj_focus = true;
        }

        else if(glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
            obj_focus = false;
        }

        if(obj_focus && objs.size() > 0) {
            glTranslated(-objs[i].getPosX(), -objs[i].getPosY(), 0);
        }

        if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && simSpeed <= maxSimSpeed - 1) {
            simSpeed++;
        }

        else if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && simSpeed > 1) {
            simSpeed--;
        }

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            break; 
        }

        bool erase = false;
        for(int ii = 0; ii < simSpeed; ii++) {
            glClear(GL_COLOR_BUFFER_BIT);
            for(auto& obj : objs) {
                if(!obj.getDeleteStatus()) {
                    obj.UpdatePos(objs);
                }
                obj.check_should_delete(objs);
                erase = true;
                if(!obj.getDeleteStatus()) {
                    obj.DrawCircle();
                }
            }
        }

        objs.erase(
            std::remove_if(objs.begin(), objs.end(), 
                [](const Object& obj) { 
                    return obj.getDeleteStatus(); 
                }
            ), 
            objs.end()
        );

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
        
        end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        if(duration.count() > 300 && obj_focus) {
            if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && i < objs.size() - 1) {
                i++;
                start = std::chrono::high_resolution_clock::now();
            } 
            if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && i > 0) {
                i--;
                start = std::chrono::high_resolution_clock::now();
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    glfwTerminate();
    return 0;
}