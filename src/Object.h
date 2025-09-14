#pragma once
#include <vector>

class Object {
public:
    std::vector<float> position;
    std::vector<float> velocity;
    int radius;
    float res;
    int mass;
    float dt;
    Object(std::vector<float> position, std::vector<float> velocity, int radius, int mass);

    void getDeltaTime(float dt);

    void DrawCircle();
};