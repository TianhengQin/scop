#include "main.hpp"

int main(int ac, char **av) {

    if (ac != 3) {
        std::cout << "scop [object] [texture]" << std::endl;
        return 1;
    }

    if (loadfile(av[1]))
        return 1;

    if (!glfwInit()) {
        std::cout << "GLFW failed" << std::endl;
        return 1;
    }

    // glchangeversion();

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
    dcam = dcamo;
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
