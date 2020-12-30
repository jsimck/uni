#include <iostream>
#include <functional>
#include <mutex>
#include "lu_cpp_11.h"
#include "../utils/timer.h"

void LUCpp11::swapCol(Mat &mat, ulong i1, ulong i2) {
    // Init threads in equal size of max HW concurrency
    std::vector<std::thread> threads(nThreads);
    const ulong N = mat.size;

    // Bind function using lambda expression to each thread
    for (size_t t = 0; t < nThreads; ++t) {
        threads[t] = std::thread(std::bind(
            [&](const ulong start, const ulong end) {
                // Start each thread for different parts of the array
                for (ulong i = start; i < end; ++i) {
                    auto tmp = mat[i][i1];
                    mat[i][i1] = mat[i][i2];
                    mat[i][i2] = tmp;
                }
            }, t * (N / nThreads), (t + 1 == nThreads) ? N : (t + 1) * (N / nThreads))
        );
    }

    // Wait for all threads to finish (join)
    for (auto &thread : threads) {
        thread.join();
    }
}

void LUCpp11::checkDiagonal(Mat &mat, Mat &permutation) {
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

void LUCpp11::computeGauss(Mat &mat, Mat &unit) {
    // Init threads array
    const ulong N = mat.size;
    std::vector<std::thread> threads(nThreads);
    auto *multipliers = new float[N + 1];

    for (ulong k = 0; k < N - 1; k++) {
        // Get upper val
        const ulong kN = N - k;
        const ulong normNThreads = (nThreads > kN) ? kN : nThreads;
        float upperVal = (mat[k][k] == 0) ? 1 : mat[k][k];

        // Calculate multipliers for each row
        const ulong nK = k + 1;
        for (size_t t = 0; t < normNThreads; ++t) {
            threads[t] = std::thread(std::bind(
                [&](const ulong start, const ulong end) {
                    for (ulong i = start; i < end; i++) {
                        multipliers[i] = -(mat[i][k] / upperVal);
                    }
                }, t * (kN / normNThreads) + nK, (t + 1 == normNThreads) ? N : (t + 1) * (kN / normNThreads) + nK)
            );
        }

        // Wait for all threads to finish (join)
        for (ulong j = 0; j < normNThreads; ++j) {
            threads[j].join();
        }

        // Calc matrices
        for (size_t t = 0; t < normNThreads; ++t) {
            threads[t] = std::thread(std::bind(
                [&](const ulong start, const ulong end) {
                    for (ulong y = start; y < end; y++) {
                        for (ulong x = 0; x < N; x++) {
                            mat[y][x] = mat[y][x] + (multipliers[y] * mat[k][x]);
                            unit[y][x] = unit[y][x] + (multipliers[y] * unit[k][x]);
                        }
                    }
                }, t * (kN / normNThreads) + nK, (t + 1 == normNThreads) ? N : (t + 1) * (kN / normNThreads) + nK)
            );
        }

        // Wait for all threads to finish (join)
        for (ulong j = 0; j < normNThreads; ++j) {
            threads[j].join();
        }
    }

    delete[] multipliers;
}

void LUCpp11::calc(Mat &mat, Mat &upper, Mat &lower, Mat &permutation) {
    // Init matrices
    Mat unit = Mat::generateUnitMatrix(mat.size);
    permutation = unit;
    lower = unit;
    upper = mat;

    // Check if there are no zeros at diagonal, if yes fix it
    Timer t1;
    checkDiagonal(upper, permutation);
    std::cout << std::endl << "checkDiagonal() took: " << t1.elapsed() << "s" << std::endl;

    // Upper matrix
    Timer t2;
    computeGauss(upper, unit);
    std::cout << "computeGauss() took: " << t2.elapsed() << "s" << std::endl;

    // Lower matrix
    Timer t3;
    computeGauss(unit, lower);
    std::cout << "computeGauss() took: " << t3.elapsed() << "s" << std::endl;
}