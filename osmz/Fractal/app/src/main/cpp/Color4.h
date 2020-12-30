#ifndef FRACTAL_COLOR4_H
#define FRACTAL_COLOR4_H

#include <stdint.h>

struct Color4 {
    union {
        struct {
            uint32_t r;
            uint32_t b;
            uint32_t g;
            uint32_t a;
        };

        uint32_t data[4];
    };

    Color4() : r(0), b(0), g(0), a(255) {}
    Color4(uint32_t r, uint32_t g, uint32_t b) : r(r), g(g), b(b), a(255) {}
    Color4(uint32_t r, uint32_t g, uint32_t b, uint32_t a) : r(r), g(g), b(b), a(a) {}
    Color4(uint32_t px);

    uint32_t abgr();
    Color4 grayscale();
    Color4 inverse();

    friend Color4 operator+(const Color4 &c1, const Color4 &c2);
    friend Color4 operator-(const Color4 &c1, const Color4 &c2);
    friend Color4 operator/(const Color4 &c1, uint32_t k);
    friend Color4 operator*(const Color4 &c1, uint32_t k);
};

#endif //FRACTAL_COLOR4_H
