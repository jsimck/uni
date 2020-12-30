#ifndef VSB_PA_LU_DECOMPOSITION_MATRIX_H
#define VSB_PA_LU_DECOMPOSITION_MATRIX_H

#include <vector>
#include <ostream>
#include <random>
#include "../utils/usings.h"

struct Mat {
private:
    static std::random_device rd;
    static std::mt19937 gen;
    static std::uniform_real_distribution<float> dis;

public:
    ulong size;
    std::vector<std::vector<float>> data;

    // Statics
    static Mat generate(ulong size, float min = 0, float max = 1.0f);
    static Mat generateUnitMatrix(ulong size);

    // Constructors
    Mat() : size(0) {}
    Mat(ulong size) : size(size) {}
    Mat(ulong size, float val);

    // Methods
    std::string toWolframFormat();

    // Overloads
    std::vector<float> &operator[](unsigned int i);
    friend std::ostream &operator<<(std::ostream &os, const Mat &mat);
};

#endif //VSB_PA_LU_DECOMPOSITION_MATRIX_H
