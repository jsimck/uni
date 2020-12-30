#include <iostream>
#include "lu_open_mp.h"

void LUOpenMP::swapCol(Mat &mat, ulong i1, ulong i2) {
    #pragma omp parallel for shared(mat) firstprivate(i1, i2)
    for (ulong i = 0; i < mat.size; ++i) {
        auto tmp = mat[i][i1];
        mat[i][i1] = mat[i][i2];
        mat[i][i2] = tmp;
    }
}

void LUOpenMP::checkDiagonal(Mat &mat, Mat &permutation) {
    const ulong N = mat.size;
    bool valid;

    do {
        valid = true;

        for (ulong i = 0; i < N; ++i) {
            if (mat[i][i] == 0) {
                // Swap cols
                for (ulong j = i + 1; j < N; j++) {
                    if (mat[j][j] != 0 && mat[i][j] != 0) {
                        swapCol(mat, i, j);
                        swapCol(permutation, i, j);
                        break;
                    }
                }
            }
        }

        // Check again
        for (ulong i = 0; i < N; ++i) {
            if (mat[i][i] == 0) {
                valid = false;
                break;
            }
        }
    } while (!valid);
}

void LUOpenMP::computeGauss(Mat &mat, Mat &unit) {
    const ulong N = mat.size;
    auto *multipliers = new float[N + 1];

    for (ulong k = 0; k < N - 1; k++) {
        // Get upper val
        float upperVal = (mat[k][k] == 0) ? 1 : mat[k][k];

        // Calculate multipliers for each row
        #pragma omp parallel for shared(multipliers) firstprivate(upperVal)
        for (ulong i = k + 1; i < N; i++) {
            multipliers[i] = -(mat[i][k] / upperVal);
        }

        // Calc mat
        #pragma omp parallel for shared(mat, unit) firstprivate(multipliers)
        for (ulong y = k + 1; y < N; y++) {
            for (ulong x = 0; x < N; x++) {
                mat[y][x] = mat[y][x] + (multipliers[y] * mat[k][x]);
                unit[y][x] = unit[y][x] + (multipliers[y] * unit[k][x]);
            }
        }
    }

    delete[] multipliers;
}

void LUOpenMP::calc(Mat &mat, Mat &upper, Mat &lower, Mat &permutation) {
    // Init matrices
    Mat unit = Mat::generateUnitMatrix(mat.size);
    permutation = unit;
    lower = unit;
    upper = mat;

    // Check if there are no zeros at diagonal, if yes fix it
    checkDiagonal(upper, permutation);

    // Upper matrix
    computeGauss(upper, unit);

    // Lower matrix
    computeGauss(unit, lower);
}