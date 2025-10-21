#pragma once
#include <vector>

struct Vec2 {
    double x, y;
    
    Vec2() : x(0), y(0) {}
    Vec2(double x, double y) : x(x), y(y) {}
    
    // Solo le operazioni base che usi nel codice
    Vec2 operator+(const Vec2& other) const { 
        return {x + other.x, y + other.y}; 
    }
    
    Vec2 operator-(const Vec2& other) const { 
        return {x - other.x, y - other.y}; 
    }
    
    Vec2 operator*(double scalar) const { 
        return {x * scalar, y * scalar}; 
    }

    Vec2 operator/(double scalar) const { 
        return {x / scalar, y / scalar}; 
    }
    
    Vec2& operator+=(const Vec2& other) { 
        x += other.x; 
        y += other.y; 
        return *this; 
    }
    
    double magnitude() const { 
        return std::sqrt(x * x + y * y); 
    }
};

struct Color {
    int r, g, b;
    Color(int x, int y, int z) : r(x), g(y), b(z) {}
};

class BHTree;

class Object {
public:
    Object(Vec2 position, Vec2 velocity, int radius, double mass, Color color);

    void DrawCircle();

    void Object::UpdatePos(const BHTree& tree);

    Vec2 Object::CalculatePullFactor(const BHTree& tree);

    void check_should_delete(std::vector<Object*>& objs);

    Vec2 getPosition() const;
    Vec2 getVelocity() const;
    double getMass(void) const;
    bool getDeleteStatus(void) const;

private:

    struct State {
        Vec2 position;
        Vec2 velocity;
    };
    
    State CalculateDerivatives(const State& state, const BHTree& tree);

    Vec2 position;
    Vec2 velocity;
    Vec2 acc;
    Color colors;
    std::vector<Vec2> prev_pos;
    int radius;
    double res;
    double mass;
    bool shouldDelete;
};