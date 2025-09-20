#include "Planets.hpp"
#include "config.hpp"  

Sun::Sun() : Object(
    { screenWidth / 2.0, screenHeight / 2.0 }, 
    { 0.0, 0.0 },                              
    30,                                       
    1.989e30                                   
) {}

Earth::Earth() : Object(
    { screenWidth / 2.0, screenHeight / 2.0 }, 
    { 0.0, 0.0},                                    
    int(12745594.0 / scaling_factor),
    5.972e24
) {}

Moon::Moon() : Object(
    { screenWidth / 2.0 + (3.844e8 / scaling_factor), screenHeight / 2.0 }, 
    { 0.0, 1019.8 }, 
    int(3474800.0 / scaling_factor),
    7.348e22
) {}
