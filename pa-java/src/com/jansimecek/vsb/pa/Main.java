package com.jansimecek.vsb.pa;

import com.jansimecek.vsb.pa.model.Mat;

public class Main {
    public static void main(String[] args) {
        // Do factorization
        LUFactorization lu = new LUFactorization(Mat.generate(3000, -100.0f, 100.0f),  1);

        // Print results
        long start = System.currentTimeMillis();
        lu.calc();
        long time = System.currentTimeMillis() - start;
        System.out.println("LU Factorization took: " + ((float) time / 1000.0f) + "s");
    }
}
