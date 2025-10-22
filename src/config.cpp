#include "config.hpp"
#include "Object.hpp"
#include <random>

int screenWidth = 800;
int screenHeight = 600;
double zoom = 1.0;
double lastMouseX = 0.0;
double lastMouseY = 0.0;
Vec2 cameraOffset(0.0, 0.0); // Aggiungi questa variabile globale per tracciare l'offset della camera
std::random_device rd;
std::mt19937 gen(rd());

GLFWwindow* StartGLFW() {
    if (!glfwInit()) {
        std::cout << "failed to initialize glfw, panic!" << std::endl;
        return nullptr;
    }
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    screenWidth = mode->width;
    screenHeight = mode->height;
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Gravity Simulator", monitor, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    return window;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    if (yoffset < 0) {
        zoom *= 0.9;  
    } else if (yoffset > 0) {
        zoom *= 1.1;
    }
    if (zoom < 0.01) zoom = 0.01;
    if (zoom > 40000.0) zoom = 40000.0;
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    lastMouseX = xpos;
    lastMouseY = ypos;
}

// Funzione per convertire coordinate schermo in coordinate mondo
Vec2 screenToWorld(double screenX, double screenY) {
    // Converti coordinate pixel in coordinate normalizzate [-1, 1]
    double ndcX = (2.0 * screenX / screenWidth) - 1.0;
    double ndcY = 1.0 - (2.0 * screenY / screenHeight);
    
    // Applica la trasformazione inversa dello zoom e della proiezione
    double viewWidth = screenWidth / zoom;
    double viewHeight = screenHeight / zoom;
    
    // La formula corretta: prendi lo spazio visibile e aggiungi l'offset
    double worldX = ndcX * (viewWidth / 2.0) + cameraOffset.x;
    double worldY = ndcY * (viewHeight / 2.0) + cameraOffset.y;
    
    return Vec2(worldX, worldY);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if ((button == GLFW_MOUSE_BUTTON_LEFT || button == GLFW_MOUSE_BUTTON_RIGHT) && action == GLFW_PRESS) {
        std::vector<Object>* objs = static_cast<std::vector<Object>*>(
            glfwGetWindowUserPointer(window)
        );
        
        // Converti le coordinate del mouse in coordinate del mondo
        Vec2 worldPos = screenToWorld(lastMouseX, lastMouseY);
        
        std::uniform_real_distribution<> v(0.0, 300.0);
        std::uniform_real_distribution<> mass(7e22, 7e25);
        std::uniform_int_distribution<> radius(10, 10);
        std::uniform_int_distribution<> color(150, 255);
        
        Vec2 vel(v(rd), v(rd));
        Color col(color(rd), color(rd), color(rd));
        double massa = mass(rd);
        int raggio = radius(rd);
        
        objs->push_back(Object(worldPos, vel, raggio, massa, col));
    }
}