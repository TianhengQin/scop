#include <iostream>
#include <cstdio>

int main() {

    float x,y,z,t;
    int a,b,c,d;

    char line[] = "v 0.232406 -1.216630 1.133818";
    // char line[] = "f 665/1/665 32/2/32 578/3/578";

    int re = std::sscanf(line, "f %d /%d /%d %d/ %d /%d %d/ %d /%d", &a, &b, &c,&c, &c, &c, &c, &c, &c);

    std::cout << "match re: " << re << std::endl;
    std::cout << a << " " << b << " " << c << std::endl;

    return 0;
}