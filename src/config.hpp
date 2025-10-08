#pragma once
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <cmath>
#include <iostream>
#include <vector>

#define PI 3.14159265358979
#define G 6.67e-11
#define MAX_PREV_POS 3000

extern double simulationSpeed;
const double scaling_factor = 3.884e8 / 25; // 36836935.1669941
const double standard_res = 100;

extern double zoom;
extern int screenWidth;
extern int screenHeight;

GLFWwindow *StartGLFW();
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void speed_up_sim();
void speed_down_sim();
