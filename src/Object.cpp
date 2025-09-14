#include "config.h"

Object::Object(std::vector<float> position, std::vector<float> velocity, int radius, int mass) {
    this->position = position;
    this->velocity = velocity;
    this->radius = radius;
    this->mass = mass;
    this->res = standard_res;
};

void Object::getDeltaTime(float dt) {
    this->dt = dt;
}

void Object::DrawCircle() {
    glBegin(GL_TRIANGLE_FAN);

    glVertex2f(position[0], position[1]);

    for (int i = 0; i <= res; ++i) {
        float angle = 2.0f * PI * i / res;
        float x = position[0] + cos(angle) * radius;
        float y = position[1] + sin(angle) * radius;
        glVertex2f(x, y);
    }

    glEnd();
}
