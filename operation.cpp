#include "scop.hpp"

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
        dcam = dcamo;
        key_r = false;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        yshift -= dshift;
    } else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        yshift += dshift;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        xshift -= dshift;
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        xshift += dshift;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        zshift -= dshift;
    } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        zshift += dshift;
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
        dcam *= 0.995f;
    } else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        dcam *= 1.005025126f;
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
