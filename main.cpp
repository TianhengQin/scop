#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <unistd.h>
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb.h"

GLuint texture;
GLuint program;
unsigned int VBO, VAO;


typedef struct vec3 {
    float x;
    float y;
    float z;
} v3;

typedef struct mat3 {
    float m[3][3];
} m3;

std::vector<v3> vertices;
std::vector<float> rendBf;
float dcam;
float theta;
float xshift;
float yshift;
float zshift;
float tratio;
m3 rotm;
bool key_esc;
bool key_r;

v3 minus(v3 a, v3 b) {

    v3 re;

    re.x = a.x - b.x;
    re.y = a.y - b.y;
    re.z = a.z - b.z;
    return re;
}

float dot(v3 a, v3 b) {
    return (a.x * b.x + a.y * b.y + a.z * b.z);
}

v3 cross(v3 a, v3 b) {
    v3 re;
    re.x = a.y * b.z - a.z * b.y;
    re.y = a.z * b.x - a.x * b.z;
    re.z = a.x * b.y - a.y * b.x;
    return re;
}

v3 normized(v3 a) {
    float	mod;
    mod = sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
	a.x = a.x / mod;
	a.y = a.y / mod;
	a.z = a.z / mod;
	return (a);
}

v3 nv(v3 a, v3 b, v3 c) {
    return normized(cross(minus(b,a),minus(c,a)));
}

void push3(int a, int b, int c, bool up) {

    v3 n;
    int l = vertices.size();

    if (a < 1 || a > l) {
        std::cout<<"!!! Achtung! Face number out of index: "<< a <<std::endl;
        return;
    }
    if (b < 1 || b > l) {
        std::cout<<"!!! Achtung! Face number out of index: "<< b <<std::endl;
        return;
    }
    if (c < 1 || c > l) {
        std::cout<<"!!! Achtung! Face number out of index: "<< c <<std::endl;
        return;
    }

    a--;
    b--;
    c--;

    n = nv(vertices[a],vertices[b],vertices[c]);

    rendBf.push_back(vertices[a].x);
    rendBf.push_back(vertices[a].y);
    rendBf.push_back(vertices[a].z);

    rendBf.push_back(n.x);
    rendBf.push_back(n.y);
    rendBf.push_back(n.z);

    if (up) {
        rendBf.push_back(0.0f);
        rendBf.push_back(0.0f);
    } else {
        rendBf.push_back(0.0f);
        rendBf.push_back(0.0f);
    }

    rendBf.push_back(vertices[b].x);
    rendBf.push_back(vertices[b].y);
    rendBf.push_back(vertices[b].z);

    rendBf.push_back(n.x);
    rendBf.push_back(n.y);
    rendBf.push_back(n.z);

    if (up) {
        rendBf.push_back(1.0f);
        rendBf.push_back(0.0f);
    } else {
        rendBf.push_back(1.0f);
        rendBf.push_back(1.0f);
    }

    rendBf.push_back(vertices[c].x);
    rendBf.push_back(vertices[c].y);
    rendBf.push_back(vertices[c].z);

    rendBf.push_back(n.x);
    rendBf.push_back(n.y);
    rendBf.push_back(n.z);

    if (up) {
        rendBf.push_back(1.0f);
        rendBf.push_back(1.0f);
    } else {
        rendBf.push_back(0.0f);
        rendBf.push_back(1.0f);
    }

}

int loadfile(char *file) {

    std::ifstream obj;
    int re;
    v3 vec;
    float w;

    std::cout<<"Loading file :"<< file <<std::endl;
    obj.open(file);
    if (!obj.is_open()) {
        std::cout<<"Failed" <<std::endl;
        return 1;
    }
    std::string line;
    std::cout<< "Loading vertices ..." <<std::endl;
    xshift = 0;
    yshift = 0;
    zshift = 0;
    while (std::getline(obj, line)) {
        if (line[0] == 'v' && line[1] == ' ') {
            re = std::sscanf(line.c_str(), "v %f %f %f %f", &vec.x, &vec.y, &vec.z, &w);
            if (re == 3) {
                xshift += vec.x;
                yshift += vec.y;
                zshift += vec.z;
                vertices.push_back(vec);
            } else if (re == 4) {
                vec.x = vec.x/w;
                vec.y = vec.y/w;
                vec.z = vec.z/w;
                xshift += vec.x;
                yshift += vec.y;
                zshift += vec.z;
                vertices.push_back(vec);
            } else {
                std::cout<<"!!! Achtung! Vertex format error: "<< line <<std::endl;
            }
        }
    }
    obj.close();

    xshift /= (float)vertices.size();
    yshift /= (float)vertices.size();
    zshift /= (float)vertices.size();

    for (std::vector<v3>::iterator it = vertices.begin(); it != vertices.end(); it++) {
        it->x -= xshift;
        it->y -= yshift;
        it->z -= zshift;
    }

    int a[15];
    obj.open(file);
    if (!obj.is_open()) {
        std::cout<<"Failed" <<std::endl;
        return 1;
    }
    std::cout<<"Loading faces ..."<<std::endl;
    while (std::getline(obj, line)) {
        if (line[0] == 'f' && line[1] == ' ') {
            re = std::sscanf(line.c_str(), "f %d %d %d %d %d", a, a+1, a+2, a+3, a+4);
            if (re == 3) {
                push3(a[0],a[1],a[2],true);
                continue;
            } else if (re == 4) {
                push3(a[0],a[1],a[2],true);
                push3(a[0],a[2],a[3],false);
                continue;
            } else if (re == 5) {
                push3(a[0],a[1],a[2],true);
                push3(a[0],a[2],a[3],false);
                push3(a[0],a[3],a[4],true);
                continue;
            }
            re = std::sscanf(line.c_str(), "f %d/%d %d/%d %d/%d %d/%d %d/%d", a, a+5, a+1, a+6, a+2, a+7, a+3, a+8, a+4, a+9);
            if (re == 6) {
                push3(a[0],a[1],a[2],true);
                continue;
            } else if (re == 8) {
                push3(a[0],a[1],a[2],true);
                push3(a[0],a[2],a[3],false);
                continue;
            } else if (re == 10) {
                push3(a[0],a[1],a[2],true);
                push3(a[0],a[2],a[3],false);
                push3(a[0],a[3],a[4],true);
                continue;
            }
            re = std::sscanf(line.c_str(), "f %d//%d %d//%d %d//%d %d//%d %d//%d", a, a+10, a+1, a+11, a+2, a+12, a+3, a+13, a+4, a+14);
            if (re == 6) {
                push3(a[0],a[1],a[2],true);
                continue;
            } else if (re == 8) {
                push3(a[0],a[1],a[2],true);
                push3(a[0],a[2],a[3],false);
                continue;
            } else if (re == 10) {
                push3(a[0],a[1],a[2],true);
                push3(a[0],a[2],a[3],false);
                push3(a[0],a[3],a[4],true);
                continue;
            }
            re = std::sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
                                            a, a+5, a+10, a+1, a+6, a+11, a+2, a+7, a+12, a+3, a+8, a+13, a+4, a+9, a+14);
            if (re == 9) {
                push3(a[0],a[1],a[2],true);
                continue;
            } else if (re == 12) {
                push3(a[0],a[1],a[2],false);
                push3(a[0],a[2],a[3],true);
                continue;
            } else if (re == 15) {
                push3(a[0],a[1],a[2],true);
                push3(a[0],a[2],a[3],false);
                push3(a[0],a[3],a[4],true);
                continue;
            }
            std::cout<<"!!! Achtung! Face format error / not supported: "<< line <<std::endl;
        }
    }
    obj.close();
    std::cout<<"Obj loaded"<<std::endl;
    return (0);
}

void glchangeversion() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

void glprintversion() {
    std::cout << "============================================================" << std::endl;
    std::cout << "Renderer:       " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version:   " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "============================================================" << std::endl;
}

int compileshader() {

    int re = 0;

    const char *vertexShader = "#version 410 core\n"
                               "layout (location = 0) in vec3 aPos;\n"
                               "layout (location = 1) in vec3 aNv;\n"
                               "layout (location = 2) in vec2 aTex;\n"
                               "out vec3 nv;\n"
                               "out vec2 tc;\n"
                               "uniform float xs;\n"
                               "uniform float ys;\n"
                               "uniform float zs;\n"
                               "uniform float c;\n"
                               "uniform mat3 rm;\n"
                               "void main() {\n"
                               "   vec3 Pos = aPos * rm;\n"
                               "   float z = Pos.z + zs;\n"
                               "   float w = (c - z);\n"
                               "   float zzz = (-10.0f-80.1f/99.9f*100.0f)/90.0f * z + 80.1f/99.9f*c;\n"
                               "   gl_Position = vec4(Pos.x + xs, Pos.y + ys, zzz, w);\n"
                               "   nv = aNv;\n"
                               "   tc = aTex;\n"
                               "}\n";
    std::cout << "Vertex Shader:\n" << vertexShader << std::endl;

    const char *fragmentShader =  "#version 410 core\n"
                                  "out vec4 FragColor;\n"
                                  "in vec3 nv;\n"
                                  "in vec2 tc;\n"
                                  "uniform sampler2D tex;\n"
                                  "uniform float tr;\n"
                                  "void main() {\n"
                                  "   float br;\n"
                                  "   vec4 cl;\n"
                                  "   float z = nv.z;\n"
                                  "   br = (z + 1.0f)/2.0f;\n"
                                  "   cl = vec4(1.0f * br, 1.0f * br, 1.0f * br, 1.0f);\n"
                                  "   FragColor = (1.0f - tr) * cl + tr * texture(tex, tc);\n"
                                  "}\n";
    std::cout << "Fragment Shader:\n" << fragmentShader << std::endl;

    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &vertexShader, NULL);
    glCompileShader(vShader);

    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &fragmentShader, NULL);
    glCompileShader(fShader);

    program = glCreateProgram();
    glAttachShader(program, vShader);
    glAttachShader(program, fShader);
    glLinkProgram(program);

    int success;
    char infoLog[512];
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fShader, 512, NULL, infoLog);
        std::cout << "Vertex shader compilation error: " << infoLog << std::endl;
        re = 1;
    }
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fShader, 512, NULL, infoLog);
        std::cout << "Fragment shader compilation error: " << infoLog << std::endl;
        re = 1;
    }
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "Linking Failed: " << infoLog << std::endl;
        re = 1;
    }
    glDeleteShader(vShader);
    glDeleteShader(fShader);
    if (!re) {
        std::cout << "Shader compiled" << std::endl;
    }
    return re;
}

void setVAO() {

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * rendBf.size(), rendBf.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    glBindVertexArray(0); 

    glEnable(GL_DEPTH_TEST);
}

void init_rot() {
    rotm.m[0][0] = 1.0f;
    rotm.m[1][1] = 1.0f;
    rotm.m[2][2] = 1.0f;
    rotm.m[0][1] = 0.0f;
    rotm.m[0][2] = 0.0f;
    rotm.m[1][0] = 0.0f;
    rotm.m[1][2] = 0.0f;
    rotm.m[2][0] = 0.0f;
    rotm.m[2][1] = 0.0f;
}

void init_pos() {
    xshift = 0.0f;
    yshift = 0.0f;
    zshift = 0.0f;
}

m3 rotx(float t) {

    m3 re;

    re.m[0][0] = 1.0f;
    re.m[1][1] = cosf(t);
    re.m[2][2] = re.m[0][0];
    re.m[0][1] = 0.0f;
    re.m[0][2] = 0.0f;
    re.m[1][0] = 0.0f;
    re.m[1][2] = -sinf(t);
    re.m[2][0] = 0.0f;
    re.m[2][1] = -re.m[1][2];

    return re;
}

m3 roty(float t) {

    m3 re;

    re.m[0][0] = cosf(t);
    re.m[1][1] = 1.0f;
    re.m[2][2] = re.m[0][0];
    re.m[0][1] = 0.0f;
    re.m[0][2] = sinf(t);
    re.m[1][0] = 0.0f;
    re.m[1][2] = 0.0f;
    re.m[2][0] = -re.m[0][2];
    re.m[2][1] = 0.0f;

    return re;
}

m3 rotz(float t) {

    m3 re;

    re.m[0][0] = cosf(t);
    re.m[1][1] = re.m[0][0];
    re.m[2][2] = 1.0f;
    re.m[0][1] = -sinf(t);
    re.m[0][2] = 0.0f;
    re.m[1][0] = -re.m[0][1];
    re.m[1][2] = 0.0f;
    re.m[2][0] = 0.0f;
    re.m[2][1] = 0.0f;

    return re;
}

m3 productm3(m3 a, m3 b) {
    m3 re;

    re.m[0][0] = a.m[0][0] * b.m[0][0] + a.m[0][1] * b.m[1][0] + a.m[0][2] * b.m[2][0];
    re.m[1][1] = a.m[1][0] * b.m[0][1] + a.m[1][1] * b.m[1][1] + a.m[1][2] * b.m[2][1];
    re.m[2][2] = a.m[2][0] * b.m[0][2] + a.m[2][1] * b.m[1][2] + a.m[2][2] * b.m[2][2];
    re.m[0][1] = a.m[0][0] * b.m[0][1] + a.m[0][1] * b.m[1][1] + a.m[0][2] * b.m[2][1];
    re.m[0][2] = a.m[0][0] * b.m[0][2] + a.m[0][1] * b.m[1][2] + a.m[0][2] * b.m[2][2];
    re.m[1][0] = a.m[1][0] * b.m[0][0] + a.m[1][1] * b.m[1][0] + a.m[1][2] * b.m[2][0];
    re.m[1][2] = a.m[1][0] * b.m[0][2] + a.m[1][1] * b.m[1][2] + a.m[1][2] * b.m[2][2];
    re.m[2][0] = a.m[2][0] * b.m[0][0] + a.m[2][1] * b.m[1][0] + a.m[2][2] * b.m[2][0];
    re.m[2][1] = a.m[2][0] * b.m[0][1] + a.m[2][1] * b.m[1][1] + a.m[2][2] * b.m[2][1];

    return re;
}


void Events(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        key_esc = true;
    } else if (key_esc) {
        std::cout<<"Exit"<<std::endl;
		glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        key_r = true;
    } else if (key_r) {
		init_pos();
        init_rot();
        dcam = 8.0f;
        key_r = false;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        yshift -= 0.05;
    } else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        yshift += 0.05;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        xshift -= 0.05;
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        xshift += 0.05;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        zshift -= 0.05;
    } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        zshift += 0.05;
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        rotm = productm3(rotm, roty(0.01));
    } else if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
        rotm = productm3(rotm, roty(-0.01));
    }
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
        rotm = productm3(rotm, rotx(0.01));
    } else if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        rotm = productm3(rotm, rotx(-0.01));
    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        rotm = productm3(rotm, rotz(0.01));
    } else if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        rotm = productm3(rotm, rotz(-0.01));
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        dcam *= 0.99f;
    } else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        dcam *= 1.010101f;
    }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
        if (tratio == 0.0f) {
            tratio = 0.01f;
        } else {
            tratio *= 1.05263158f;
            if (tratio > 1.0f) {
                tratio = 1.0f;
            }
        }
    } else if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
        if (tratio < 0.01f) {
            tratio = 0.0f;
        } else {
            tratio *= 0.95f;
        }
    }
}

int loadtexture(char *file) {

    GLuint texid;
    GLuint texUni;
    std::vector<unsigned char> byte;
    std::ifstream tex;

    std::cout << "Loading texture : " <<file<< std::endl;
    int w, h, cn;

    unsigned char* bytes = stbi_load(file, &w, &h, &cn, 0);

    tex.open(file);
    if (!tex.is_open()) {
        std::cout<<"Failed" <<std::endl;
        return 1;
    }

    char bf;
    int base;
    tex.get(bf);
    if (bf != 'B') {
        std::cout << "BMP format error" << std::endl;
        return 1;
    }
    tex.get(bf);
    if (bf != 'M') {
        std::cout << "BMP format error" << std::endl;
        return 1;
    }

    for (int i=0;i<8;i++) {
        tex.get(bf);
    }

    int offset = 0;
    base = 1;
    for (int i=0;i<4;i++) {
        tex.get(bf);
        offset += (int)*(unsigned char *)&bf * base;
        base *= 256;
        std::cout << offset << std::endl;
    }

    for (int i=0;i<4;i++) {
        tex.get(bf);
    }

    int width = 0;
    base = 1;
    for (int i=0;i<4;i++) {
        tex.get(bf);
        width += (int)*(unsigned char *)&bf * base;
        base *= 256;
        std::cout << width << std::endl;
    }

    int height = 0;
    base = 1;
    for (int i=0;i<4;i++) {
        tex.get(bf);
        height += (int)*(unsigned char *)&bf * base;
        base *= 256;
        std::cout << height << std::endl;
    }

    offset -= 26;
    for (int i=0;i<offset;i++) {
        tex.get(bf);
        std::cout << (int)*(unsigned char *)&bf << std::endl;
    }

    // int pad = (4 - width * 3 % 4) % 4;
    // for (int j=0;j<height;j++) {
    //     for (int i=0;i<width;i++) {
    //         tex.get(bf);
    //         byte.push_back(*(unsigned char *)&bf);
    //         std::cout << (int)*(unsigned char *)&bf << " ";
    //         tex.get(bf);
    //         byte.push_back(*(unsigned char *)&bf);
    //         std::cout << (int)*(unsigned char *)&bf<< " ";
    //         tex.get(bf);
    //         byte.push_back(*(unsigned char *)&bf);
    //         std::cout << (int)*(unsigned char *)&bf<< "|";
    //     }
    //     for (int i=0;i<pad;i++) {
    //         tex.get(bf);
    //         byte.push_back(0);
    //         std::cout << "pad ";
    //     }
    //     std::cout << std::endl;

    // }

    while (tex.get(bf)) {
        byte.push_back(*(unsigned char *)&bf);
    }
    std::cout << byte.size() << std::endl;
    std::cout << w << h << std::endl;
    std::cout << width << height << std::endl;

    int pad = (4 - width * 3 % 4) % 4 + width * 3;
    unsigned char sw;
    int beg;
    for (int j=0;j<height;j++) {
        for (int i=0;i<width;i++) {
            beg = pad * j + i * 3;
            sw = byte[beg];
            byte[beg] = byte[beg + 2];
            byte[beg + 2] = sw;
        }
    }

    // textbyte.data();
    // std::cout << w << " " << h << " " << cn << std::endl;
    // int text;
    // for (text = 0; text < 50; text++) {
    //     std::cout << text << " " << (int)bytes[text*3] << " " << (int)bytes[text*3+1] << " " << (int)bytes[text*3+2] << std::endl;
    // }

    glGenTextures(1, &texid);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texid);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, byte.data());

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(bytes);

    glBindTexture(GL_TEXTURE_2D, 0);

    texUni = glGetUniformLocation(program, "tex");
    glUseProgram(program);
    glUniform1i(texUni, 0);
    std::cout<<"Texture loaded"<<std::endl;
    texture = texid;

    return 0;
}

int main(int ac, char **av) {

    if (ac != 3) {
        std::cout << "scop [object] [texture]" << std::endl;
        return 1;
    }

    if (loadfile(av[1]))
        return 1;

    if (!glfwInit())
        return 1;

    glchangeversion();

    GLFWwindow *window = glfwCreateWindow(720, 720, "scop", NULL, NULL);

    if (!window) {
        std::cout << "Windows loading failed" << std::endl;
        glfwTerminate();
        return 2;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "GLAD loading failed" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return 2;
    }

    glprintversion();

    if (compileshader()) {
        glDeleteProgram(program);
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    setVAO();

    theta = 0.0f;
    dcam = 8.0f;
    key_esc = false;
    key_r = false;
    tratio = 0.0f;

    init_pos();
    init_rot();

    GLuint xs;
    GLuint ys;
    GLuint zs;
    GLuint rm;
    GLuint cm;
    GLuint tr;

    int bfsize = rendBf.size()/8;

    if (loadtexture(av[2])) {
        glDeleteProgram(program);
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    rm = glGetUniformLocation(program, "rm");
    xs = glGetUniformLocation(program, "xs");
    ys = glGetUniformLocation(program, "ys");
    zs = glGetUniformLocation(program, "zs");
    cm = glGetUniformLocation(program, "c");
    tr = glGetUniformLocation(program, "tr");


    while (!glfwWindowShouldClose(window)) {

        glClearColor(0.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Events(window);

        glUniformMatrix3fv(rm, 1, GL_FALSE, &rotm.m[0][0]);
        glUniform1f(xs, xshift);
        glUniform1f(ys, yshift);
        glUniform1f(zs, zshift);
        glUniform1f(cm, dcam);
        glUniform1f(tr, tratio);

        glUseProgram(program);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, bfsize);

        glfwSwapBuffers(window);

        glfwPollEvents();

    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteTextures(1, &texture);
    glDeleteProgram(program);   
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
