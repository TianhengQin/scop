#include "scop.hpp"

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
                                  "   br = (z + 1.5f)/2.5f;\n"
                                  "   cl = vec4(1.0f * br, 1.0f * br, 1.0f * br, 1.0f);\n"
                                  "   FragColor = (1.0f - tr) * cl + tr * texture(tex, tc);\n"
                                  "}\n";

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
