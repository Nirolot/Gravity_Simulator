#include "config.hpp"
#include "BHTree.hpp"
#include "Object.hpp"

Object::Object(std::vector<double> position, std::vector<double> velocity, int radius, double mass, std::vector<int> colors)
    : position(position), velocity(velocity), radius(radius), mass(mass), colors(colors) {
    this->acc = {0.0, 0.0};
    this->res = standard_res;
    this->shouldDelete = false;
    this->prev_pos.push_back(position);
};

double Object::getPosX(void) const {
    return position[0];
}

double Object::getPosY(void) const{
    return position[1];
}

double Object::getVelX(void) const {
    return velocity[0];
}

double Object::getVelY(void) const {
    return velocity[1];
}

double Object::getMass(void) const {
    return mass;
}

bool Object::getDeleteStatus(void) const {
    return shouldDelete;
}

void Object::DrawCircle() {
    glPointSize(1.0f);
    glBegin(GL_POINTS);
    for(int i = 0; i < prev_pos.size() - 1; i++) {
        float alpha = i / float(MAX_PREV_POS);
        glColor4f(1.0, 1.0, 1.0, alpha);
        glVertex2f(prev_pos[i][0], prev_pos[i][1]);
    }
    glEnd();

    if(!prev_pos.empty()) {
        auto& p = prev_pos.back();
        glColor3ub(colors[0], colors[1], colors[2]);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(p[0], p[1]);
        for (int j = 0; j <= res; ++j) {
            double angle = 2.0 * PI * j / res;
            double x = p[0] + cos(angle) * radius;
            double y = p[1] + sin(angle) * radius;
            glVertex2f(x, y);
        }
        glEnd();
    }

    if(prev_pos.size() >= MAX_PREV_POS) {
        prev_pos.erase(prev_pos.begin());
    }
}

void Object::UpdatePos(const BHTree& tree) {

    std::vector<double> state = {position[0], position[1], velocity[0], velocity[1]};

    std::vector<double> k1 = CalculateDerivatives(state, tree);
    
    std::vector<double> state2 = {
        state[0] + 0.5 * dt * k1[0] / scaling_factor,
        state[1] + 0.5 * dt * k1[1] / scaling_factor,
        state[2] + 0.5 * dt * k1[2],
        state[3] + 0.5 * dt * k1[3]
    };

    std::vector<double> k2 = CalculateDerivatives(state2, tree);
    
    std::vector<double> state3 = {
        state[0] + 0.5 * dt * k2[0] / scaling_factor,
        state[1] + 0.5 * dt * k2[1] / scaling_factor,
        state[2] + 0.5 * dt * k2[2],
        state[3] + 0.5 * dt * k2[3]
    };

    std::vector<double> k3 = CalculateDerivatives(state3, tree);
    
    std::vector<double> state4 = {
        state[0] + dt * k3[0] / scaling_factor,
        state[1] + dt * k3[1] / scaling_factor,
        state[2] + dt * k3[2],
        state[3] + dt * k3[3]
    };

    std::vector<double> k4 = CalculateDerivatives(state4, tree);
    
    position[0] += dt * (k1[0] + 2*k2[0] + 2*k3[0] + k4[0]) / (6.0 * scaling_factor);
    position[1] += dt * (k1[1] + 2*k2[1] + 2*k3[1] + k4[1]) / (6.0 * scaling_factor);
    
    velocity[0] += dt * (k1[2] + 2*k2[2] + 2*k3[2] + k4[2]) / 6.0;
    velocity[1] += dt * (k1[3] + 2*k2[3] + 2*k3[3] + k4[3]) / 6.0;
    
    acc = CalculatePullFactor(tree, {});

    prev_pos.push_back(position);
}

void Object::check_should_delete(std::vector<Object>& objs) {
    for (auto& obj : objs) {
        if (&obj != this && !obj.shouldDelete && !this->shouldDelete) {
            double dx = obj.position[0] - position[0];
            double dy = obj.position[1] - position[1];
            double distance = hypot(dx, dy);

            if(distance < this->radius + obj.radius) {
                obj.shouldDelete = true;
                this->shouldDelete = true;
            }
        }
    }
}

std::vector<double> Object::CalculateDerivatives(const std::vector<double>& state, const BHTree& tree) {
    std::vector<double> derivatives(4);
    derivatives[0] = state[2];
    derivatives[1] = state[3];
    std::vector<double> tempAcc = CalculatePullFactor(tree, {state[0], state[1]});
    derivatives[2] = tempAcc[0];
    derivatives[3] = tempAcc[1];
    return derivatives;
}

std::vector<double> Object::CalculatePullFactor(const BHTree& tree, const std::vector<double>& pos) {
    std::vector<double> currentPos = pos.empty() ? position : pos;
    return tree.calculateForce(*this);
}

