#include "config.hpp"
#include "Object.hpp"

Object::Object(std::vector<double> position, std::vector<double> velocity, int radius, double mass, std::vector<int> colors)
    : position(position), velocity(velocity), radius(radius), mass(mass), colors(colors) {
    this->acc = {0.0, 0.0};
    this->res = standard_res;
    this->shouldDelete = false;
    this->prev_pos.push_back(position);
};

double Object::getPosX(void) {
    return position[0];
}

double Object::getPosY(void) {
    return position[1];
}

double Object::getVelX(void) {
    return velocity[0];
}

double Object::getVelY(void) {
    return velocity[0];
}

double Object::getMass(void) {
    return mass;
}

bool Object::getDeleteStatus(void) {
    return shouldDelete;
}

void Object::DrawCircle() {
    for(int i = 0; i < prev_pos.size(); i++) {    
        glBegin(GL_TRIANGLE_FAN);

        glVertex2f(prev_pos[i][0], prev_pos[i][1]);

        int raggio;

        if(i == prev_pos.size() - 1) {
            raggio = radius;
            glColor3ub(colors[0], colors[1], colors[2]);
        }
        else {
            raggio = 5;
            glColor3ub(255, 255, 255);  // * (i / float(MAX_PREV_POS)) 
        };

        for (int j = 0; j <= res; ++j) {
            double angle = 2.0f * PI * j / res;
            double x = prev_pos[i][0] + cos(angle) * raggio;
            double y = prev_pos[i][1] + sin(angle) * raggio;
            glVertex2f(x, y);
        }

        glEnd();
    }
    if(prev_pos.size() >= MAX_PREV_POS) {
        prev_pos.erase(prev_pos.begin());
    }
}

void Object::UpdatePos(const std::vector<Object>& objs) {

    std::vector<double> state = {position[0], position[1], velocity[0], velocity[1]};

    std::vector<double> k1 = CalculateDerivatives(state, objs);
    
    std::vector<double> state2 = {
        state[0] + 0.5 * simulationSpeed * k1[0] / scaling_factor,
        state[1] + 0.5 * simulationSpeed * k1[1] / scaling_factor,
        state[2] + 0.5 * simulationSpeed * k1[2],
        state[3] + 0.5 * simulationSpeed * k1[3]
    };

    std::vector<double> k2 = CalculateDerivatives(state2, objs);
    
    std::vector<double> state3 = {
        state[0] + 0.5 * simulationSpeed * k2[0] / scaling_factor,
        state[1] + 0.5 * simulationSpeed * k2[1] / scaling_factor,
        state[2] + 0.5 * simulationSpeed * k2[2],
        state[3] + 0.5 * simulationSpeed * k2[3]
    };

    std::vector<double> k3 = CalculateDerivatives(state3, objs);
    
    std::vector<double> state4 = {
        state[0] + simulationSpeed * k3[0] / scaling_factor,
        state[1] + simulationSpeed * k3[1] / scaling_factor,
        state[2] + simulationSpeed * k3[2],
        state[3] + simulationSpeed * k3[3]
    };

    std::vector<double> k4 = CalculateDerivatives(state4, objs);
    
    position[0] += simulationSpeed * (k1[0] + 2*k2[0] + 2*k3[0] + k4[0]) / (6.0 * scaling_factor);
    position[1] += simulationSpeed * (k1[1] + 2*k2[1] + 2*k3[1] + k4[1]) / (6.0 * scaling_factor);
    
    velocity[0] += simulationSpeed * (k1[2] + 2*k2[2] + 2*k3[2] + k4[2]) / 6.0;
    velocity[1] += simulationSpeed * (k1[3] + 2*k2[3] + 2*k3[3] + k4[3]) / 6.0;
    
    acc = CalculatePullFactor(objs, {});

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

