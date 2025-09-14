#include "config.h"

int screenWidth = 800;   // valori di default
int screenHeight = 600;

void updateDeltaTime(std::vector<Object> objs, float deltaTime) {
    for(auto& obj : objs) {
        obj.getDeltaTime(deltaTime);
    }
}

void drawObjects(std::vector<Object> objs) {
    for(auto& obj : objs) {
        obj.DrawCircle();
    }
}