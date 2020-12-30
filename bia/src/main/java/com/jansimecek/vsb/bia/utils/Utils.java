package com.jansimecek.vsb.bia.utils;

public final class Utils {
    public static float square(float x) {
        return x * x;
    }

    public static long factorial(long n) {
        if (n == 0) return 1;
        if (n == 1) return 1;

        long result = n;
        for (long i = n - 1; i > 0; i--) {
            result *= i;
        }

        return result;
    }
}
