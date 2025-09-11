#include "scop.hpp"

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
