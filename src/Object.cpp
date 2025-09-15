#include "config.h"
#include "Object.h"

Object::Object(std::vector<double> position, std::vector<double> velocity, int radius, double mass) {
    this->position = position;
    this->velocity = velocity;
    this->radius = radius;
    this->mass = mass;
    this->res = standard_res;
    this->shouldDelete = false;
};

void Object::getDeltaTime(double dt) {
    this->dt = 8.64e3;
}

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

void Object::UpdatePos() {
    position[0] += velocity[0] * dt / scaling_factor;
    position[1] += velocity[1] * dt / scaling_factor;
}

void Object::CalculatePullFactor(std::vector<Object> objs) {
    for(auto& obj : objs) {
        if(&obj != this && !obj.shouldDelete) {
            double dx = (obj.position[0] - this->position[0]) * scaling_factor;
            double dy = (obj.position[1] - this->position[1]) * scaling_factor;
            double distance = hypot(dx, dy);

            if(distance > 0) {
                double a = G * obj.mass / (distance * distance);
                std::cout << "acc: " << a << " - Distance: {" << dx << ", " << dy << "}" << std::endl;
                double ax = a * (dx / distance);
                double ay = a * (dy / distance);

                this->velocity[0] += ax * this->dt;
                this->velocity[1] += ay * this->dt;
            }
        }
    }
}
