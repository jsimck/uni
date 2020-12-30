package com.jansimecek.vsb.pa;

import com.jansimecek.vsb.pa.model.Mat;
import com.jansimecek.vsb.pa.model.Tuple;
import javafx.util.Pair;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class LUFactorization {
    private Mat matrix;
    private Mat upper;
    private Mat lower;
    private Mat permutation;
    private int nThreads;

    public LUFactorization() {
        this(Mat.generate(1000, -100.0f, 100.0f), 0);
    }

    public LUFactorization(Mat matrix, int nThreads) {
        setMatrix(matrix);
        this.nThreads = (nThreads == 0) ? Runtime.getRuntime().availableProcessors() : nThreads;
    }

    public void setMatrix(Mat matrix) {
        this.matrix = matrix;
    }

    private Mat swapColParallel(Mat mat, int i1, int i2) {
        // Init executor service
        final int N = mat.size;
        ExecutorService executorService = Executors.newFixedThreadPool(nThreads);

        for (int t = 0; t < nThreads; t++) {
            final int start = t * (N / nThreads);
            final int end = (t + 1 == nThreads) ? N : (t + 1) * (N / nThreads);

            executorService.submit(() -> {
                for (int i = start; i < end; ++i) {
                    float tmp = mat.data[i][i1];
                    mat.data[i][i1] = mat.data[i][i2];
                    mat.data[i][i2] = tmp;
                }
            });
        }

        // Join
        executorService.shutdown();

        return mat;
    }

    private Mat swapCol(Mat mat, int i1, int i2) {
        for (int i = 0; i < mat.size; ++i) {
            float tmp = mat.data[i][i1];
            mat.data[i][i1] = mat.data[i][i2];
            mat.data[i][i2] = tmp;
        }

        return mat;
    }

    private Tuple<Mat, Mat> checkDiagonal(Mat mat) {
        Mat permutation = Mat.generateUnitMatrix(mat.size);
        final int N = mat.size;
        boolean valid;

        do {
            valid = true;

            for (int i = 0; i < N; ++i) {
                if (mat.data[i][i] == 0) {
                    // Swap cols
                    for (int j = i + 1; j < N; j++) {
                        if (mat.data[j][j] != 0 && mat.data[i][j] != 0) {
                            if (nThreads == 1) {
                                swapCol(mat, i, j);
                                swapCol(permutation, i, j);
                            } else {
                                swapColParallel(mat, i, j);
                                swapColParallel(permutation, i, j);
                            }
                            break;
                        }
                    }
                }
            }

            // Check again
            for (int i = 0; i < N; ++i) {
                if (mat.data[i][i] == 0) {
                    valid = false;
                    break;
                }
            }
        } while (!valid);

        return new Tuple<>(mat, permutation);
    }

    private Tuple<Mat, Mat> computeGauss(Mat mat, Mat unit) {
        for (int k = 0; k < mat.size - 1; k++) {
            // Get upper val
            float multipliers[] = new float[mat.size + k + 1];
            float upperVal = (mat.data[k][k] == 0) ? 1 : mat.data[k][k];

            // Calculate multipliers for each row
            for (int i = k + 1; i < mat.size; i++) {
                multipliers[i] = -(mat.data[i][k] / upperVal);
            }

            // Calc mat
            for (int y = k + 1; y < mat.size; y++) {
                for (int x = 0; x < mat.size; x++) {
                    mat.data[y][x] = mat.data[y][x] + (multipliers[y] * mat.data[k][x]);
                    unit.data[y][x] = unit.data[y][x] + (multipliers[y] * unit.data[k][x]);
                }
            }
        }

        return new Tuple<>(mat, unit);
    }

    private Tuple<Mat, Mat> computeGaussParallel(Mat mat, Mat unit) {
        // Init threads array
        final int N = mat.size;

        for (int k = 0; k < N - 1; k++) {
            // Get upper val
            final int kN = N - k;
            final int normNThreads = (nThreads > kN) ? kN : nThreads;

            // Calculate multipliers for each row
            float multipliers[] = new float[N + k + 1];
            float upperVal = (mat.data[k][k] == 0) ? 1 : mat.data[k][k];

            // Init executor
            ExecutorService executorService = Executors.newFixedThreadPool(normNThreads);
            final int nK = k + 1;
            final int cK = k;

            for (int t = 0; t < normNThreads; t++) {
                final int start = t * (kN / normNThreads) + nK;
                final int end = (t + 1 == normNThreads) ? N : (t + 1) * (kN / normNThreads) + nK;

                executorService.submit(() -> {
                    for (int i = start; i < end; i++) {
                        multipliers[i] = -(mat.data[i][cK] / upperVal);
                    }
                });
            }

            // Join
            executorService.shutdown();

            // Init executor
            executorService = Executors.newFixedThreadPool(normNThreads);

            for (int t = 0; t < normNThreads; t++) {
                final int start = t * (kN / normNThreads) + nK;
                final int end = (t + 1 == normNThreads) ? N : (t + 1) * (kN / normNThreads) + nK;

                executorService.submit(() -> {
                    for (int y = start; y < end; y++) {
                        for (int x = 0; x < N; x++) {
                            mat.data[y][x] = mat.data[y][x] + (multipliers[y] * mat.data[cK][x]);
                            unit.data[y][x] = unit.data[y][x] + (multipliers[y] * unit.data[cK][x]);
                        }
                    }
                });
            }

            // Join
            executorService.shutdown();
        }

        return new Tuple<>(mat, unit);
    }

    public void calc() {
        // Check if there are no zeros at diagonal, if yes fix it
        upper = new Mat(matrix);
        Tuple<Mat, Mat> permutationMatrices = checkDiagonal(upper);
        upper = permutationMatrices.getFirst();
        permutation = permutationMatrices.getSecond();

        // Upper matrix, returns pair of computed upper matrix and inverse lower matrix
        Mat unit = Mat.generateUnitMatrix(upper.size);
        Tuple<Mat, Mat> upperMatrices;

        if (nThreads == 1) {
            upperMatrices = computeGauss(upper, unit);
        } else {
            upperMatrices = computeGaussParallel(upper, unit);
        }

        upper = upperMatrices.getFirst();
        Mat lowerInv = upperMatrices.getSecond();

        // Lower matrix, returns pair of computed lower matrix and unit
        lower = Mat.generateUnitMatrix(lowerInv.size);
        Tuple<Mat, Mat> lowerMatrices;

        if (nThreads == 1) {
            lowerMatrices = computeGauss(lowerInv, lower);
        } else {
            lowerMatrices = computeGaussParallel(lowerInv, lower);
        }

        lower = lowerMatrices.getSecond();
    }

    public void print() {
        System.out.println("Matrix");
        System.out.println(matrix.toString());
        System.out.println("Upper");
        System.out.println(upper.toString());
        System.out.println("Lower");
        System.out.println(lower.toString());
        System.out.println("Permutation");
        System.out.println(permutation.toString());
    }
}