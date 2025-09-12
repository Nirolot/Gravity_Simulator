#pragma once
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <cmath>
#include <iostream>
#include <vector>
//#include <algorithm>

const float screenWidth = 800.0f;
const float screenHeight = 600.0f;
const float simulationSpeed = 0.01f;
const float scaling_factor = 1e3;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#define G 6.67e-11
#endif