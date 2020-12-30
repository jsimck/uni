#ifndef MATRIX_MULTIPLICATION_MAT_H
#define MATRIX_MULTIPLICATION_MAT_H

#include <random>
#include <ostream>

using ulong = unsigned long;
using uint = unsigned int;

struct Mat {
private:
    static std::random_device seed;
    static std::mt19937 rnd;

public:
    uint rows, cols;
    ulong **data;

    static Mat zeros(uint rows, uint cols);
    static Mat random(uint rows, uint cols);

    Mat(uint rows, uint cols);
    ~Mat();

    void reset();

    ulong* operator[](uint row);
    friend std::ostream &operator<<(std::ostream &os, const Mat &mat);
};

#endif //MATRIX_MULTIPLICATION_MAT_H
