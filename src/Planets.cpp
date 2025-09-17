#include "Planets.h"
#include "config.h"  

Sun::Sun() : Object(
    { screenWidth / 2.0, screenHeight / 2.0 }, // posizione al centro
    { 0.0, 0.0 },                              // velocità nulla
    30,                                        // raggio di disegno
    1.989e30                                   // massa del sole
) {}

Earth::Earth() : Object(
    { screenWidth / 2.0 + 4072, screenHeight / 2.0 }, // a destra del sole
    { 0.0, 29664.79394},                                    // velocità iniziale verso l’alto
    7,
    5.972e24
) {}

Moon::Moon() : Object(
    { screenWidth / 2.0 + 4072 + 11, screenHeight / 2.0 }, // vicino alla Terra
    { 0.0, 29664.79394 + 1019.8 },                                   // un po’ più veloce
    1,
    7.348e22
) {}
