#include "BHTree.hpp"
#include <cmath>

BHTree::BHTree(TQuadrant quad)
    : coord_quad(quad), center_of_mass({0.0, 0.0}), tot_mass(0.0), obj(nullptr), is_leaf(true) {
    // Inizializza un nodo vuoto con il quadrante specificato
}

void BHTree::subdivide() {
    double mid_x = (coord_quad.min_x + coord_quad.max_x) / 2.0;
    double mid_y = (coord_quad.min_y + coord_quad.max_y) / 2.0;
    
    NW = std::make_unique<BHTree>(TQuadrant(coord_quad.min_x, mid_y, mid_x, coord_quad.max_y));
    NE = std::make_unique<BHTree>(TQuadrant(mid_x, mid_y, coord_quad.max_x, coord_quad.max_y));
    SW = std::make_unique<BHTree>(TQuadrant(coord_quad.min_x, coord_quad.min_y, mid_x, mid_y));
    SE = std::make_unique<BHTree>(TQuadrant(mid_x, coord_quad.min_y, coord_quad.max_x, mid_y));
    
    is_leaf = false;
}

void BHTree::insert(Object* new_obj) {
    if (!new_obj || new_obj->getDeleteStatus()) return;

    if (!obj && is_leaf && tot_mass == 0.0) {
        obj = new_obj;
        tot_mass = new_obj->getMass();
        center_of_mass = {new_obj->getPosX(), new_obj->getPosY()};
        return;
    }

    if (is_leaf && obj) {
        Object* existing_obj = obj;
        obj = nullptr;  // Invalida il puntatore prima di suddividere
        subdivide();
        if (existing_obj && !existing_obj->getDeleteStatus()) {
            insert_to_subquadrant(existing_obj);
        }
    }

    if (new_obj && !new_obj->getDeleteStatus()) {
        insert_to_subquadrant(new_obj);
    }

    if (new_obj && !new_obj->getDeleteStatus()) {
        double new_mass = new_obj->getMass();
        double total_mass = tot_mass + new_mass;
        center_of_mass[0] = (center_of_mass[0] * tot_mass + new_obj->getPosX() * new_mass) / total_mass;
        center_of_mass[1] = (center_of_mass[1] * tot_mass + new_obj->getPosY() * new_mass) / total_mass;
        tot_mass = total_mass;
    }
}

void BHTree::insert_to_subquadrant(Object* obj) {
    if (!obj || obj->getDeleteStatus()) return;

    double x = obj->getPosX();
    double y = obj->getPosY();
    double mid_x = (coord_quad.min_x + coord_quad.max_x) / 2.0;
    double mid_y = (coord_quad.min_y + coord_quad.max_y) / 2.0;

    if (x < mid_x) {
        if (y < mid_y) SW->insert(obj);
        else NW->insert(obj);
    } else {
        if (y < mid_y) SE->insert(obj);
        else NE->insert(obj);
    }
}

std::vector<double> BHTree::calculateForce(const Object& target, double theta) const {
    std::vector<double> force = {0.0, 0.0};
    if (!obj || is_leaf && tot_mass == 0.0) return force; // Nodo vuoto o senza oggetto valido

    double dx = center_of_mass[0] - target.getPosX();
    double dy = center_of_mass[1] - target.getPosY();
    double distance = std::hypot(dx, dy);
    double quad_size = coord_quad.max_x - coord_quad.min_x;

    if (is_leaf && obj && obj != &target && !obj->getDeleteStatus()) {
        if (distance > 0) {
            double a = G * obj->getMass() / (distance * distance);
            force[0] = a * (dx / distance);
            force[1] = a * (dy / distance);
        }
        return force;
    }

    if (distance > 0 && quad_size / distance < theta) {
        double a = G * tot_mass / (distance * distance);
        force[0] = a * (dx / distance);
        force[1] = a * (dy / distance);
        return force;
    }

    if (!is_leaf) {
        auto force_NW = NW->calculateForce(target, theta);
        auto force_NE = NE->calculateForce(target, theta);
        auto force_SW = SW->calculateForce(target, theta);
        auto force_SE = SE->calculateForce(target, theta);
        force[0] = force_NW[0] + force_NE[0] + force_SW[0] + force_SE[0];
        force[1] = force_NW[1] + force_NE[1] + force_SW[1] + force_SE[1];
    }

    return force;
}

void BHTree::invalidateObject() {
    obj = nullptr;
    tot_mass = 0.0;
    center_of_mass = {0.0, 0.0};
}