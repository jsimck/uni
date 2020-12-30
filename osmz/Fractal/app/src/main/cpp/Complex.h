#ifndef FRACTAL_COMPLEX_H
#define FRACTAL_COMPLEX_H

template<typename T>
struct Complex {
    T r;
    T i;

    Complex(T r, T i) : r(r), i(i) {}

    friend Complex operator+(const Complex &z1, const Complex &z2) {
        return Complex(z1.r + z2.r, z1.i + z2.i);
    }

    friend Complex operator-(const Complex &z1, const Complex &z2) {
        return Complex(z1.r - z2.r, z1.i - z2.i);
    }

    friend Complex operator*(const Complex &z1, const Complex &z2) {
        return Complex(z1.r * z2.r - z1.i * z2.i , z1.r * z2.i + z1.i * z2.r);
    }
};

#endif //FRACTAL_COMPLEX_H
