#include <omp.h>
#include "multiplication.h"

// 2D
void ::mult::multiplyRCV(Mat &a, Mat &b, Mat &result) {
    #pragma omp parallel for shared(a, b, result)
    for(int row = 0; row < 1500; ++row) {
        for (int col = 0; col < 1500; ++col) {
            for (int val = 0; val < 1500; ++val) {
                result.data[row][col] += a.data[row][val] * b.data[val][col];
            }
        }
    }
}

void ::mult::multiplyRVC(Mat &a, Mat &b, Mat &result) {
    #pragma omp parallel for default(none) shared(a, b, result)
    for(int row = 0; row < a.rows; ++row) {
        for (int val = 0; val < a.cols; ++val) {
            for (int col = 0; col < b.cols; ++col) {
                result[row][col] += a[row][val] * b[val][col];
            }
        }
    }
}

void ::mult::multiplyCRV(Mat &a, Mat &b, Mat &result) {
    #pragma omp parallel for default(none) shared(a, b, result)
    for (int col = 0; col < b.cols; ++col) {
        for(int row = 0; row < a.rows; ++row) {
            for (int val = 0; val < a.cols; ++val) {
                result[row][col] += a[row][val] * b[val][col];
            }
        }
    }
}

void ::mult::multiplyCVR(Mat &a, Mat &b, Mat &result) {
    #pragma omp parallel for default(none) shared(a, b, result)
    for (int col = 0; col < b.cols; ++col) {
        for (int val = 0; val < a.cols; ++val) {
            for(int row = 0; row < a.rows; ++row) {
                result[row][col] += a[row][val] * b[val][col];
            }
        }
    }
}

void ::mult::multiplyVCR(Mat &a, Mat &b, Mat &result) {
    #pragma omp parallel for default(none) shared(a, b, result)
    for (int val = 0; val < a.cols; ++val) {
        for (int col = 0; col < b.cols; ++col) {
            for(int row = 0; row < a.rows; ++row) {
                result[row][col] += a[row][val] * b[val][col];
            }
        }
    }
}

void ::mult::multiplyVRC(Mat &a, Mat &b, Mat &result) {
    #pragma omp parallel for default(none) shared(a, b, result)
    for (int val = 0; val < a.cols; ++val) {
        for(int row = 0; row < a.rows; ++row) {
            for (int col = 0; col < b.cols; ++col) {
                result[row][col] += a[row][val] * b[val][col];
            }
        }
    }
}