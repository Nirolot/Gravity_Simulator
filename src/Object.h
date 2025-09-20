#pragma once
#include <vector>

class Object {
public:
    std::vector<double> position;
    std::vector<double> velocity;
    std::vector<double> acc;
    int radius;
    double res;
    double mass;
    double dt;
    bool shouldDelete;
    Object(std::vector<double> position, std::vector<double> velocity, int radius, double mass);

    void DrawCircle();

    void UpdatePos(const std::vector<Object>& objs);

    std::vector<double> CalculatePullFactor(const std::vector<Object>& objs, const std::vector<double>& pos);

    std::vector<double> CalculateDerivatives(const std::vector<double>& state, const std::vector<Object>& objs);
};