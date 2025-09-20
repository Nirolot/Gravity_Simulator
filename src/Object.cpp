#include "config.h"
#include "Object.h"

Object::Object(std::vector<double> position, std::vector<double> velocity, int radius, double mass)
    : position(position), velocity(velocity), radius(radius), mass(mass) {
    this->acc = {0.0, 0.0};
    this->res = standard_res;
    this->shouldDelete = false;
    this->dt = simulationSpeed;
};

void Object::DrawCircle() {
    glBegin(GL_TRIANGLE_FAN);

    glVertex2f(position[0], position[1]);

    for (int i = 0; i <= res; ++i) {
        double angle = 2.0f * PI * i / res;
        double x = position[0] + cos(angle) * radius;
        double y = position[1] + sin(angle) * radius;
        glVertex2f(x, y);
    }

    glEnd();
}

void Object::UpdatePos(const std::vector<Object>& objs) {

    std::vector<double> state = {position[0], position[1], velocity[0], velocity[1]};

    std::vector<double> k1 = CalculateDerivatives(state, objs);
    
    std::vector<double> state2 = {
        state[0] + 0.5 * dt * k1[0] / scaling_factor,
        state[1] + 0.5 * dt * k1[1] / scaling_factor,
        state[2] + 0.5 * dt * k1[2],
        state[3] + 0.5 * dt * k1[3]
    };

    std::vector<double> k2 = CalculateDerivatives(state2, objs);
    
    std::vector<double> state3 = {
        state[0] + 0.5 * dt * k2[0] / scaling_factor,
        state[1] + 0.5 * dt * k2[1] / scaling_factor,
        state[2] + 0.5 * dt * k2[2],
        state[3] + 0.5 * dt * k2[3]
    };

    std::vector<double> k3 = CalculateDerivatives(state3, objs);
    
    std::vector<double> state4 = {
        state[0] + dt * k3[0] / scaling_factor,
        state[1] + dt * k3[1] / scaling_factor,
        state[2] + dt * k3[2],
        state[3] + dt * k3[3]
    };

    std::vector<double> k4 = CalculateDerivatives(state4, objs);
    
    position[0] += dt * (k1[0] + 2*k2[0] + 2*k3[0] + k4[0]) / (6.0 * scaling_factor);
    position[1] += dt * (k1[1] + 2*k2[1] + 2*k3[1] + k4[1]) / (6.0 * scaling_factor);
    
    velocity[0] += dt * (k1[2] + 2*k2[2] + 2*k3[2] + k4[2]) / 6.0;
    velocity[1] += dt * (k1[3] + 2*k2[3] + 2*k3[3] + k4[3]) / 6.0;
    
    acc = CalculatePullFactor(objs, {});
}

std::vector<double> Object::CalculateDerivatives(const std::vector<double>& state, const std::vector<Object>& objs) {
    std::vector<double> derivatives(4);

    derivatives[0] = state[2];
    derivatives[1] = state[3];

    std::vector<double> tempAcc = CalculatePullFactor(objs, {state[0], state[1]});

    derivatives[2] = tempAcc[0];  
    derivatives[3] = tempAcc[1];

    return derivatives;
}

std::vector<double> Object::CalculatePullFactor(const std::vector<Object>& objs, const std::vector<double>& pos) {
    std::vector<double> acc = {0.0, 0.0};

    std::vector<double> currentPos = pos.empty() ? position : pos;

    for (const auto& obj : objs) {
        if (&obj != this && !obj.shouldDelete) {
            double dx = (obj.position[0] - currentPos[0]) * scaling_factor;
            double dy = (obj.position[1] - currentPos[1]) * scaling_factor;
            double distance = hypot(dx, dy);

            if (distance > 0) {
                double a = G * obj.mass / (distance * distance);
                acc[0] += a * (dx / distance);
                acc[1] += a * (dy / distance);
            }
        }
    }
    return acc;
}
