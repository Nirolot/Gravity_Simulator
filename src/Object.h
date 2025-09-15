#pragma once
#include <vector>

class Object {
public:
    std::vector<double> position;
    std::vector<double> velocity;
    int radius;
    double res;
    double mass;
    double dt;
    bool shouldDelete;
    Object(std::vector<double> position, std::vector<double> velocity, int radius, double mass);

    void getDeltaTime(double dt);

    void DrawCircle();

    void UpdatePos();

    void CalculatePullFactor(std::vector<Object> objs);
};