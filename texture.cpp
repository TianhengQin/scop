#include "scop.hpp"

int loadtexture(char *file) {

    GLuint texid;
    GLuint texUni;
    std::vector<unsigned char> byte;
    std::ifstream tex;

    std::cout << "Loading texture : " <<file<< std::endl;

    tex.open(file);
    if (!tex.is_open()) {
        std::cout<<"Failed" <<std::endl;
        return 1;
    }

    char bf = 0;
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
        if (!tex.get(bf)) {
            tex.close();
            std::cout << "BMP format error" << std::endl;
            return 1;
        }
    }

    int offset = 0;
    base = 1;
    for (int i=0;i<4;i++) {
        if (!tex.get(bf)) {
            tex.close();
            std::cout << "BMP format error" << std::endl;
            return 1;
        }
        offset += (int)*(unsigned char *)&bf * base;
        base *= 256;
    }

    for (int i=0;i<4;i++) {
        if (!tex.get(bf)) {
            tex.close();
            std::cout << "BMP format error" << std::endl;
            return 1;
        }
    }

    int width = 0;
    base = 1;
    for (int i=0;i<4;i++) {
        if (!tex.get(bf)) {
            tex.close();
            std::cout << "BMP format error" << std::endl;
            return 1;
        }
        width += (int)*(unsigned char *)&bf * base;
        base *= 256;
    }

    int height = 0;
    base = 1;
    for (int i=0;i<4;i++) {
        if (!tex.get(bf)) {
            tex.close();
            std::cout << "BMP format error" << std::endl;
            return 1;
        }
        height += (int)*(unsigned char *)&bf * base;
        base *= 256;
    }

    offset -= 26;
    for (int i=0;i<offset;i++) {
        if (!tex.get(bf)) {
            tex.close();
            std::cout << "BMP format error" << std::endl;
            return 1;
        }
    }

    byte.clear();
    while (tex.get(bf)) {
        byte.push_back(*(unsigned char *)&bf);
    }
    tex.close();

    int pad = (4 - width * 3 % 4) % 4 + width * 3;
    if ((unsigned long)(pad * height) != byte.size()) {
        std::cout << "BMP size error" << std::endl;
        return 1;
    }
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

    glGenTextures(1, &texid);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texid);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, byte.data());

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    texUni = glGetUniformLocation(program, "tex");
    glUseProgram(program);
    glUniform1i(texUni, 0);
    std::cout<<"Texture loaded"<<std::endl;
    texture = texid;

    return 0;
}
