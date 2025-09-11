#ifndef SCOP_HPP
# define SCOP_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <unistd.h>
#include <fstream>

extern GLuint texture;
extern GLuint program;
extern unsigned int VBO;
extern unsigned int VAO;

typedef struct vec3 {
    float x;
    float y;
    float z;
} v3;

typedef struct mat3 {
    float m[3][3];
} m3;

extern std::vector<v3> vertices;
extern std::vector<float> rendBf;
extern float dcam;
extern float dcamo;
extern float theta;
extern float xshift;
extern float yshift;
extern float zshift;
extern float dshift;
extern float tratio;
extern m3 rotm;
extern bool key_esc;
extern bool key_r;

v3 minus(v3 a, v3 b);
v3 cross(v3 a, v3 b);
v3 normized(v3 a);
v3 nv(v3 a, v3 b, v3 c);
m3 rotx(float t);
m3 roty(float t);
m3 rotz(float t);
m3 productm3(m3 a, m3 b);

#endif
