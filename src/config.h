#pragma once
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>

#define PI 3.14159265358979
#define G 6.67e-11

const double simulationSpeed = 0.01f;
const double scaling_factor = 7.55e5;
const double standard_res = 50;

extern int screenWidth;
extern int screenHeight;

GLFWwindow* StartGLFW();