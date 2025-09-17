#include "Planets.h"
#include "config.h"  

Sun::Sun() : Object(
    { screenWidth / 2.0, screenHeight / 2.0 }, 
    { 0.0, 0.0 },                              
    30,                                       
    1.989e30                                   
) {}

Earth::Earth() : Object(
    { screenWidth / 2.0 + 4072, screenHeight / 2.0 }, 
    { 0.0, 29664.79394},                                    
    7,
    5.972e24
) {}

Moon::Moon() : Object(
    { screenWidth / 2.0 + 4072 + 11, screenHeight / 2.0 }, 
    { 0.0, 29664.79394 + 1019.8 }, 
    1,
    7.348e22
) {}
