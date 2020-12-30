#ifndef VSB_PA_LU_DECOMPOSITION_LU_CPP_11_H
#define VSB_PA_LU_DECOMPOSITION_LU_CPP_11_H

#include <thread>
#include "lu.h"

class LUCpp11 : public LU {
private:
    size_t nThreads;

    void swapCol(Mat &mat, ulong i1, ulong i2);
    void checkDiagonal(Mat &mat, Mat &permutation);
    void computeGauss(Mat &mat, Mat &unit);
public:
    LUCpp11() : nThreads(std::thread::hardware_concurrency()) {}
    LUCpp11(size_t nThreads) : nThreads(nThreads) {}

    void calc(Mat &mat, Mat &upper, Mat &lower, Mat &permutation) override;
};

#endif //VSB_PA_LU_DECOMPOSITION_LU_CPP_11_H
