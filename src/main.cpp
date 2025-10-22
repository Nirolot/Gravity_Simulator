#include "config.hpp"
#include "BHTree.hpp"
#include "Object.hpp"
#include "Planets.hpp"

#include <chrono>

const int maxSimSpeed = 80;

int main() {

    GLFWwindow* window = StartGLFW();
    if (!window) {
        std::cout << "NOOO"; 
        return -1;
    }
        
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, 0, screenHeight, -1, 1);
    glMatrixMode(GL_MODELVIEW);
/*
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
*/
    Sun sun;
    std::vector<Object> objs = {};

    glfwSetWindowUserPointer(window, &objs);

    int i = 0, simSpeed = 1;
    bool obj_focus = true;

    auto start = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();

    while (!glfwWindowShouldClose(window)) {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        double viewWidth = screenWidth / zoom;
        double viewHeight = screenHeight / zoom;
        glOrtho(-viewWidth / 2.0, viewWidth / 2.0, -viewHeight / 2.0, viewHeight / 2.0, -1, 1);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glScaled(zoom, zoom, 1.0);

        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
            obj_focus = true;
        }
        else if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
            obj_focus = false;
        }

        if (obj_focus && objs.size() > 0) {
            Vec2 focusPos = objs[i].getPosition();
            cameraOffset = focusPos; // Aggiorna l'offset della camera
            glTranslated(-focusPos.x, -focusPos.y, 0);
        } else {
            cameraOffset = Vec2(0.0, 0.0); // Reset quando non segui nessuno
        }

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && simSpeed <= maxSimSpeed - 1) {
            simSpeed++;
        }
        else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && simSpeed > 1) {
            simSpeed--;
        }

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            break; 
        }

        TQuadrant quad(-screenWidth, -screenHeight, screenWidth, screenHeight);
        BHTree tree(quad);
        for (auto& obj : objs) {
            if (!obj.getDeleteStatus()) {
                tree.insert(&obj);
            }
        }

        glClear(GL_COLOR_BUFFER_BIT);
        std::vector<Object*> active_objs;
        for (auto& obj : objs) {
            if (!obj.getDeleteStatus()) {
                active_objs.push_back(&obj);
            }
        }

        for (auto* obj : active_objs) {
            obj->UpdatePos(tree);
            obj->check_should_delete(active_objs);
            obj->DrawCircle(); 
        }

        objs.erase(
            std::remove_if(objs.begin(), objs.end(), 
                [](const Object& obj) { return obj.getDeleteStatus(); }
            ), 
            objs.end()
        );
        
        end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        if (duration.count() > 300 && obj_focus) {
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && i < objs.size() - 1) {
                i++;
                start = std::chrono::high_resolution_clock::now();
            } 
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && i > 0) {
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