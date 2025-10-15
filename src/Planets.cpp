#include "Planets.hpp"
#include "config.hpp"

Sun::Sun() : Object(
    { screenWidth / 2.0, screenHeight / 2.0 },
    { 0.0, 0.0 },
    150,
    1.989e30,
    {255, 255, 0}
) {}

Mercury::Mercury() : Object(
    { screenWidth / 2.0 + (57.9e9 / scaling_factor), screenHeight / 2.0 },
    { 0.0, 47400.0 },
    5, // int(2440e3 / scaling_factor)
    3.285e23,
    {169, 169, 169}
) {}

Venus::Venus() : Object(
    { screenWidth / 2.0 + (108.2e9 / scaling_factor), screenHeight / 2.0 },
    { 0.0, 35020.0 },
    12, // int(6051e3 / scaling_factor)
    4.867e24,
    {255, 204, 102}
) {}

Earth::Earth() : Object(
    { screenWidth / 2.0 + (149.6e9 / scaling_factor), screenHeight / 2.0 },
    { 0.0, 29799.51 },
    14,
    5.972e24,
    {0, 200, 0}
) {}

Moon::Moon() : Object(
    { screenWidth / 2.0 + (149.6e9 / scaling_factor) + (384.4e6 / scaling_factor), screenHeight / 2.0 },
    { 0.0, 29799.51 + 1019.8 },
    4,
    7.348e22,
    {255, 255, 255}
) {}

Mars::Mars() : Object(
    { screenWidth / 2.0 + (227.9e9 / scaling_factor), screenHeight / 2.0 },
    { 0.0, 24077.0 },
    8, // int(3389e3 / scaling_factor)
    6.39e23,
    {255, 69, 0}
) {}

Jupiter::Jupiter() : Object(
    { screenWidth / 2.0 + (778.5e9 / scaling_factor), screenHeight / 2.0 },
    { 0.0, 13070.0 },
    30, // int(69911e3 / scaling_factor)
    1.898e27,
    {255, 153, 102}
) {}

Saturn::Saturn() : Object(
    { screenWidth / 2.0 + (1.433e12 / scaling_factor), screenHeight / 2.0 },
    { 0.0, 9680.0 },
    25, // int(58232e3 / scaling_factor)
    5.683e26,
    {210, 180, 140}
) {}

Uranus::Uranus() : Object(
    { screenWidth / 2.0 + (2.872e12 / scaling_factor), screenHeight / 2.0 },
    { 0.0, 6800.0 },
    20, // int(25362e3 / scaling_factor)
    8.681e25,
    {173, 216, 230}
) {}

Neptune::Neptune() : Object(
    { screenWidth / 2.0 + (4.495e12 / scaling_factor), screenHeight / 2.0 },
    { 0.0, 5430.0 },
    19, // int(24622e3 / scaling_factor)
    1.024e26,
    {0, 0, 255}
) {}
