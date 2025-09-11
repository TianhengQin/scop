#ifndef GLOBAL_H
# define GLOBAL_H

#include "scop.hpp"

GLuint texture;
GLuint program;
unsigned int VBO;
unsigned int VAO;

std::vector<v3> vertices;
std::vector<float> rendBf;
float dcam;
float dcamo;
float theta;
float xshift;
float yshift;
float zshift;
float dshift;
float tratio;
m3 rotm;
bool key_esc;
bool key_r;

#endif
