#include "color4.h"
#include "stdio.h"

// --- Operátory ------
Color4 operator+(const Color4 &c1, const Color4 &c2) {
    return Color4(c1.r + c2.r, c1.g + c2.g, c1.b + c2.b, c1.a + c2.a);
}

Color4 operator*(const Color4 &c, const float a) {
    return Color4(a * c.r, a * c.g, a * c.b, a * c.a);
}

Color4 operator*(const float a, const Color4 &c) {
    return Color4(a * c.r, a * c.g, a * c.b, a * c.a);
}

Color4 operator*(const Vector3 &v, const Color4 &c) {
    return Color4(v.x * c.r, v.y * c.g, v.z * c.b, c.a);
}

Color4 operator*(const Color4 &u, const Color4 &v) {
    return Color4(u.r * v.r, u.g * v.g, u.b * v.b, u.a * v.a);
}

Color4 operator/(const Color4 &u, const float a) {
    return Color4(u.r / a, u.g / a, u.b / a, u.a / a);
}

void operator+=(Color4 &c1, const Color4 &c2) {
    c1.r += c2.r;
    c1.g += c2.g;
    c1.b += c2.b;
    c1.a += c2.a;
}

void operator/=(Color4 &c, const float a) {
    c.r /= a;
    c.g /= a;
    c.b /= a;
    c.a /= a;
}

void Color4::Print() {
    printf("rgba(%0.3f, %0.3f, %0.3f, %0.3f)\n", r, g, b, a);
}
