#ifndef MATRIX_MULTIPLICATION_MULTIPLICATION_H
#define MATRIX_MULTIPLICATION_MULTIPLICATION_H

#include <random>
#include "mat.h"

namespace mult {
    // 2D
    void multiplyRCV(Mat &a, Mat &b, Mat &result);
    void multiplyRVC(Mat &a, Mat &b, Mat &result);
    void multiplyCRV(Mat &a, Mat &b, Mat &result);
    void multiplyCVR(Mat &a, Mat &b, Mat &result);
    void multiplyVCR(Mat &a, Mat &b, Mat &result);
    void multiplyVRC(Mat &a, Mat &b, Mat &result);
}

#endif //MATRIX_MULTIPLICATION_MULTIPLICATION_H
