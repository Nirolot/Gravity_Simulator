#include "BHTree.hpp"
#include <cmath>

BHTree::BHTree(TQuadrant quad)
    : coord_quad(quad), center_of_mass(0.0, 0.0), tot_mass(0.0), obj(nullptr), is_leaf(true) {
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

    // Nodo vuoto
    if (!obj && is_leaf && tot_mass == 0.0) {
        obj = new_obj;
        tot_mass = new_obj->getMass();
        center_of_mass = new_obj->getPosition();  // <-- Molto più pulito!
        return;
    }

    // Nodo con un oggetto → suddividi
    if (is_leaf && obj) {
        Object* existing_obj = obj;
        obj = nullptr;
        subdivide();
        if (existing_obj && !existing_obj->getDeleteStatus()) {
            insert_to_subquadrant(existing_obj);
        }
    }

    // Inserisci nel sottoalbero
    if (new_obj && !new_obj->getDeleteStatus()) {
        insert_to_subquadrant(new_obj);
    }

    // Aggiorna centro di massa (formula del baricentro)
    if (new_obj && !new_obj->getDeleteStatus()) {
        double new_mass = new_obj->getMass();
        double total_mass = tot_mass + new_mass;
        
        // Prima: brutto con indici
        // center_of_mass[0] = (center_of_mass[0] * tot_mass + new_obj->getPosX() * new_mass) / total_mass;
        
        // Dopo: elegante con Vec2!
        center_of_mass = (center_of_mass * tot_mass + new_obj->getPosition() * new_mass) / total_mass;
        
        tot_mass = total_mass;
    }
}

void BHTree::insert_to_subquadrant(Object* obj) {
    if (!obj || obj->getDeleteStatus()) return;

    Vec2 pos = obj->getPosition();  // <-- Più pulito!
    double mid_x = (coord_quad.min_x + coord_quad.max_x) / 2.0;
    double mid_y = (coord_quad.min_y + coord_quad.max_y) / 2.0;

    if (pos.x < mid_x) {
        if (pos.y < mid_y) SW->insert(obj);
        else NW->insert(obj);
    } else {
        if (pos.y < mid_y) SE->insert(obj);
        else NE->insert(obj);
    }
}

Vec2 BHTree::calculateForce(const Object& target, double theta) const {
    Vec2 force(0.0, 0.0);
    
    if (is_leaf && tot_mass == 0.0) return force;

    // Calcola distanza
    Vec2 delta = (center_of_mass - target.getPosition()) * scaling_factor;
    double distance = delta.magnitude();
    double quad_size = (coord_quad.max_x - coord_quad.min_x) * scaling_factor;

    // Caso 1: nodo foglia con un oggetto
    if (is_leaf && obj && obj != &target && !obj->getDeleteStatus()) {
        if (distance > 0) {
            double a = G * obj->getMass() / (distance * distance);
            force = delta * (a / distance);  // <-- Operatore * fa tutto!
        }
        return force;
    }

    // Caso 2: nodo lontano → approssima
    if (distance > 0 && quad_size / distance < theta) {
        double a = G * tot_mass / (distance * distance);
        force = delta * (a / distance);
        return force;
    }

    // Caso 3: nodo vicino → ricorsione
    if (!is_leaf) {
        force = NW->calculateForce(target, theta) +
                NE->calculateForce(target, theta) +
                SW->calculateForce(target, theta) +
                SE->calculateForce(target, theta);
    }

    return force;
}

void BHTree::invalidateObject() {
    obj = nullptr;
    tot_mass = 0.0;
    center_of_mass = Vec2(0.0, 0.0);
}