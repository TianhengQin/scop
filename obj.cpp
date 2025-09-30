#include "scop.hpp"

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

    std::cout<<"Loading file : "<< file <<std::endl;
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
    dcamo = 0;
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

    dcamo = 0.0f;
    for (std::vector<v3>::iterator it = vertices.begin(); it != vertices.end(); it++) {
        it->x -= xshift;
        it->y -= yshift;
        it->z -= zshift;
        if (abs(it->x) > dcamo) {
            dcamo = abs(it->x);
        }
        if (abs(it->y) > dcamo) {
            dcamo = abs(it->y);
        }
        if (abs(it->z) > dcamo) {
            dcamo = abs(it->z);
        }
    }
    dcamo *= 2.0f;
    dshift = dcamo / 200.0f;

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
