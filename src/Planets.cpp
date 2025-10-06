#include "Planets.hpp"
#include "config.hpp"  

Sun::Sun() : Object(
    { screenWidth / 2.0, screenHeight / 2.0 }, 
    { 0.0, 0.0 },                              
    30,                                       
    1.989e30,
    {127, 127, 127}                               
) {}

Earth::Earth() : Object(
    { screenWidth / 2.0, screenHeight / 2.0 }, 
    { 0.0, 0.0},                                    
    int(6378137.0 / scaling_factor),
    5.972e24,
    {0, 200, 0}
) {}

Moon::Moon() : Object(
    { screenWidth / 2.0 + (3.844e8 / scaling_factor), screenHeight / 2.0 }, 
    { 0.0, 1019.8 }, 
    int(1738e3 / scaling_factor),
    7.348e22,
    {255, 255, 255}
) {}
