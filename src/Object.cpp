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

    // Verlet integration
    position[0] += dt * (velocity[0] + 0.5 * acc[0] * dt) / scaling_factor;
    position[1] += dt * (velocity[1] + 0.5 * acc[1] * dt) / scaling_factor;

    std::vector<double> newAcc = CalculatePullFactor(objs);

    velocity[0] += 0.5 * dt * (acc[0] + newAcc[0]);
    velocity[1] += 0.5 * dt * (acc[1] + newAcc[1]);

    acc = newAcc;
}

std::vector<double> Object::CalculatePullFactor(const std::vector<Object>& objs) {
    std::vector<double> acc = {0.0, 0.0};

    for (const auto& obj : objs) {
        if (&obj != this && !obj.shouldDelete) {
            double dx = (obj.position[0] - this->position[0]) * scaling_factor;
            double dy = (obj.position[1] - this->position[1]) * scaling_factor;
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
