#pragma once
#include "config.hpp"
#include "Object.hpp"
#include <memory>
#include <vector>

struct TQuadrant {
    double min_x, min_y, max_x, max_y;
    TQuadrant(double min_x, double min_y, double max_x, double max_y)
        : min_x(min_x), min_y(min_y), max_x(max_x), max_y(max_y) {}
    TQuadrant() {
        min_x = 0, min_y = 0, max_x = 0, max_y = 0;
    }
};

class BHTree {
private:
    TQuadrant coord_quad;               // Limiti del quadrante
    Vec2 center_of_mass;                // Centro di massa (x, y)
    double tot_mass;                    // Massa totale
    Object* obj;                        // Oggetto (solo per nodi foglia), può essere nullptr
    bool is_leaf;                       // True se il nodo è una foglia
    std::unique_ptr<BHTree> NW;         // Sotto-quadrante nord-ovest
    std::unique_ptr<BHTree> NE;         // Sotto-quadrante nord-est
    std::unique_ptr<BHTree> SW;         // Sotto-quadrante sud-ovest
    std::unique_ptr<BHTree> SE;         // Sotto-quadrante sud-est

    void subdivide();

    void insert_to_subquadrant(Object* obj);

public:
    BHTree(TQuadrant quad);

    // Disabilita il costruttore di copia
    BHTree(const BHTree&) = delete;

    // Disabilita l'operatore di assegnazione
    BHTree& operator=(const BHTree&) = delete;

    void insert(Object* new_obj);

    Vec2 calculateForce(const Object& target, double theta = 1) const;

    void invalidateObject();
};