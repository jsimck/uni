#ifndef VSB_PA_LU_DECOMPOSITION_LU_H
#define VSB_PA_LU_DECOMPOSITION_LU_H

#include "../types/mat.h"

class LU {
public:
    virtual void calc(Mat &mat, Mat &upper, Mat &lower, Mat &permutation) = 0;
};

#endif //VSB_PA_LU_DECOMPOSITION_LU_H
