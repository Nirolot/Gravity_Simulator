#include "config.hpp"
#include "BHTree.hpp"
#include "Object.hpp"
#include <math.h>

Object::Object(Vec2 position, Vec2 velocity, int radius, double mass, Color color)
    : position(position), velocity(velocity), radius(radius), mass(mass), colors(colors)
{
    this->acc = Vec2(0.0, 0.0);
    this->res = standard_res;
    this->shouldDelete = false;
    this->prev_pos.push_back(position);
};

Vec2 Object::getPosition() const
{
    return position;
}

Vec2 Object::getVelocity() const
{
    return velocity;
}

double Object::getMass(void) const
{
    return mass;
}

bool Object::getDeleteStatus(void) const
{
    return shouldDelete;
}

void Object::DrawCircle()
{
    glPointSize(1.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < prev_pos.size() - 1; i++)
    {
        float alpha = i / float(MAX_PREV_POS);
        glColor4f(1.0, 1.0, 1.0, alpha);
        glVertex2f(prev_pos[i].x, prev_pos[i].y);
    }
    glEnd();

    if (!prev_pos.empty())
    {
        const Vec2 &p = prev_pos.back();
        glColor3ub(colors.r, colors.g, colors.b);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(p.x, p.y);
        for (int j = 0; j <= res; ++j)
        {
            double angle = 2.0 * PI * j / res;
            double x = p.x + cos(angle) * radius;
            double y = p.y + sin(angle) * radius;
            glVertex2f(x, y);
        }
        glEnd();
    }

    if (prev_pos.size() >= MAX_PREV_POS)
    {
        prev_pos.erase(prev_pos.begin());
    }
}

void Object::UpdatePos(const BHTree &tree)
{

    State state{position, velocity};

    State k1 = CalculateDerivatives(state, tree);

    State state2{
        state.position + k1.position * (0.5 * dt / scaling_factor),
        state.velocity + k1.velocity * (0.5 * dt)};
    State k2 = CalculateDerivatives(state2, tree);

    State state3{
        state.position + k2.position * (0.5 * dt / scaling_factor),
        state.velocity + k2.velocity * (0.5 * dt)};
    State k3 = CalculateDerivatives(state3, tree);

    State state4{
        state.position + k3.position * (dt / scaling_factor),
        state.velocity + k3.velocity * dt};
    State k4 = CalculateDerivatives(state4, tree);

    // Aggiorna posizione e velocità
    position += (k1.position + k2.position * 2.0 + k3.position * 2.0 + k4.position) * (dt / (6.0 * scaling_factor));

    velocity += (k1.velocity + k2.velocity * 2.0 + k3.velocity * 2.0 + k4.velocity) * (dt / 6.0);

    acc = CalculatePullFactor(tree);
    prev_pos.push_back(position);
}

void Object::check_should_delete(std::vector<Object *> &objs)
{
    for (auto *obj : objs)
    {
        if (obj != this && !obj->shouldDelete && !this->shouldDelete)
        {
            Vec2 delta = obj->position - position;
            double distance = delta.magnitude();

            if (distance < this->radius + obj->radius)
            {
                double sumM = this->mass + obj->mass;
                Vec2 deltaPos = this->position - obj->position;  // Δx
                Vec2 deltaVel = this->velocity - obj->velocity;  // Δv

                double dotProduct = deltaVel.x * deltaPos.x + deltaVel.y * deltaPos.y;
                double distanceSquared = deltaPos.x * deltaPos.x + deltaPos.y * deltaPos.y;

                std::cout << "this: " << this->velocity.x << ", " << this->velocity.y << "\n";
                std::cout << "obj: " << obj->velocity.x << ", " << obj->velocity.y << "\n";

                if (distanceSquared < 1e-10) {
                    double circleDist = obj->radius + this->radius - distance;
                    if(this->position.x > obj->position.x) {
                        this->position.x += circleDist / 2.0;
                        obj->position.x -= circleDist / 2.0;
                    }

                    else {
                        this->position.x -= circleDist / 2.0;
                        obj->position.x += circleDist / 2.0;
                    }

                    if(this->position.y > obj->position.y) {
                        this->position.y -= circleDist / 2.0;
                        obj->position.y += circleDist / 2.0;
                    }

                    else {
                        this->position.y += circleDist / 2.0;
                        obj->position.y -= circleDist / 2.0;
                    }
                    return;
                };

                // Formula della collisione elastica 2D:
                // v1' = v1 - (2*m2 / (m1+m2)) * (<v1-v2, x1-x2> / |x1-x2|²) * (x1-x2)
                double factor1 = (2.0 * obj->mass / sumM) * (dotProduct / distanceSquared);
                this->velocity = this->velocity - deltaPos * factor1;

                // Per l'altro oggetto, inverti i delta
                Vec2 deltaPos2 = obj->position - this->position;
                Vec2 deltaVel2 = obj->velocity - this->velocity;
                double dotProduct2 = deltaVel2.x * deltaPos2.x + deltaVel2.y * deltaPos2.y;

                double factor2 = (2.0 * this->mass / sumM) * (dotProduct2 / distanceSquared);
                obj->velocity = obj->velocity - deltaPos2 * factor2;

                std::cout << "this: " << this->velocity.x << ", " << this->velocity.y << "\n";
                std::cout << "obj: " << obj->velocity.x << ", " << obj->velocity.y << "\n";
            }
        }
    }
}

Object::State Object::CalculateDerivatives(const State &state, const BHTree &tree)
{
    State derivatives;
    derivatives.position = state.velocity;
    derivatives.velocity = CalculatePullFactor(tree);
    return derivatives;
}

Vec2 Object::CalculatePullFactor(const BHTree &tree)
{
    return tree.calculateForce(*this);
}
