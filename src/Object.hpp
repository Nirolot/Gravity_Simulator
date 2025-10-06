#pragma once
#include <vector>

class Object {
public:
    Object(std::vector<double> position, std::vector<double> velocity, int radius, double mass, std::vector<int> colors);

    void DrawCircle();

    void UpdatePos(const std::vector<Object>& objs);

    std::vector<double> CalculatePullFactor(const std::vector<Object>& objs, const std::vector<double>& pos);

    std::vector<double> CalculateDerivatives(const std::vector<double>& state, const std::vector<Object>& objs);

    void check_should_delete(std::vector<Object>& objs);

    double getPosX(void);
    double getPosY(void);
    double getVelX(void);
    double getVelY(void);
    double getMass(void);
    bool getDeleteStatus(void);

private:
    std::vector<double> position;
    std::vector<double> velocity;
    std::vector<double> acc;
    std::vector<int> colors;
    std::vector<std::vector<double>> prev_pos;
    int radius;
    double res;
    double mass;
    double dt;
    bool shouldDelete;
};