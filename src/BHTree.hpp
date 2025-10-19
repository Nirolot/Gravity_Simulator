#pragma once
#include "config.hpp"
#include "Object.hpp"
#include <memory>
#include <vector>

struct TQuadrant {
    double min_x, min_y, max_x, max_y;
    TQuadrant(double min_x, double min_y, double max_x, double max_y)
        : min_x(min_x), min_y(min_y), max_x(max_x), max_y(max_y) {}
};

class BHTree {
private:
    TQuadrant coord_quad;               // Limiti del quadrante
    std::vector<double> center_of_mass; // Centro di massa (x, y)
    double tot_mass;                    // Massa totale
    Object* obj;                        // Oggetto (solo per nodi foglia), può essere nullptr
    bool is_leaf;                       // True se il nodo è una foglia
    std::unique_ptr<BHTree> NW;         // Sotto-quadrante nord-ovest
    std::unique_ptr<BHTree> NE;         // Sotto-quadrante nord-est
    std::unique_ptr<BHTree> SW;         // Sotto-quadrante sud-ovest
    std::unique_ptr<BHTree> SE;         // Sotto-quadrante sud-est

    // Suddivide il quadrante in quattro sotto-quadranti
    void subdivide();

    // Inserisce un oggetto nel sotto-quadrante appropriato
    void insert_to_subquadrant(Object* obj);

public:
    BHTree(TQuadrant quad);

    // Disabilita il costruttore di copia
    BHTree(const BHTree&) = delete;

    // Disabilita l'operatore di assegnazione
    BHTree& operator=(const BHTree&) = delete;

    // Inserisce un oggetto nell'albero
    void insert(Object* new_obj);

    // Calcola la forza gravitazionale su un oggetto
    std::vector<double> calculateForce(const Object& target, double theta = 0.5) const;

    // Invalida il puntatore all'oggetto (utile se l'oggetto viene eliminato)
    void invalidateObject();
};