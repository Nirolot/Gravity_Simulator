#pragma once
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <cmath>
#include <iostream>
#include <vector>

#define PI 3.14159265358979
#define G 6.67e-11
#define MAX_PREV_POS 100

const double dt = 100;
const double scaling_factor = 1e6;
const double standard_res = 100;

extern double zoom;
extern int screenWidth;
extern int screenHeight;
extern struct Vec2; // Forward declaration

// Aggiungi questa riga:
extern Vec2 cameraOffset;

GLFWwindow *StartGLFW();
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);