#ifndef VSB_PA_LU_DECOMPOSITION_OPEN_MP_H
#define VSB_PA_LU_DECOMPOSITION_OPEN_MP_H

#include "../types/mat.h"
#include "lu.h"

class LUOpenMP : public LU {
private:
    void swapCol(Mat &mat, ulong i1, ulong i2);
    void checkDiagonal(Mat &mat, Mat &permutation);
    void computeGauss(Mat &mat, Mat &unit);
public:
    void calc(Mat &mat, Mat &upper, Mat &lower, Mat &permutation) override;
};

#endif //VSB_PA_LU_DECOMPOSITION_OPEN_MP_H
