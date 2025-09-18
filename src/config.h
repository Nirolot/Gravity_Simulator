#pragma once
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <cmath>
#include <iostream>
#include <vector>

#define PI 3.14159265358979
#define G 6.67e-11

const double simulationSpeed = 8.64e3;
const double scaling_factor = 3.844e8 / 200.0; // 36836935.1669941
const double standard_res = 50;

extern double zoom;
extern int screenWidth;
extern int screenHeight;

GLFWwindow* StartGLFW();
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
