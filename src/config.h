#pragma once
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include "Object.h"

#define PI 3.14159265358979
#define G 6.67e-11

const float simulationSpeed = 0.01f;
const float scaling_factor = 7.55e5;
const float standard_res = 50;

extern int screenWidth;
extern int screenHeight;

void updateDeltaTime(std::vector<Object> objs, float deltaTime);
void drawObjects(std::vector<Object> objs);