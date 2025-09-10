
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <unistd.h>
#include <fstream>


int main() {

    std::ifstream tax;
    std::string bf;

    tax.open("resources/cat4.bmp");

    if (!tax.is_open()) {
        std::cout<<"Failed" <<std::endl;
        return 1;
    }

    // bf << tax.rdbuf();

    char rbf[2000];

    tax.read(rbf, 2000);

    unsigned char * ubf = (unsigned char *)rbf;


    for (int i = 138; i<2000; i++) {
        std::cout << (int)ubf[i] << " ";
    }
    // std::cout << (unsigned int)((unsigned char)rbf[11]) << std::endl;
    std::cout  << std::endl;

    tax.close();
}
