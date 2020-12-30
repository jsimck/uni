package com.jansimecek.vsb.pa.model;

import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class Mat implements Cloneable {
    static private Random rand;
    public int size;
    public float[][] data;

    static {
        rand = new Random();
    }

    public Mat(int size) {
        this.size = size;
        data = new float[this.size][this.size];
    }

    public Mat() {
        this(0);
    }

    public Mat(int size, float val) {
        this(size);

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                data[i][j] = val;
            }
        }
    }

    public Mat(Mat matrix) {
        this.size = matrix.size;
        data = new float[this.size][this.size];

        for (int i = 0; i < this.size; i++) {
            System.arraycopy(matrix.data[i], 0, data[i], 0, this.size);
        }
    }

    public static Mat generate(int size, float min, float max) {
        Mat mat = new Mat(size);

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                mat.data[i][j] = (float) Math.round((max - min) * rand.nextFloat() + min);
            }
        }

        return mat;
    }

    public static Mat generateUnitMatrix(int size) {
        Mat mat = new Mat(size, 0);

        for (int i = 0; i < size; i++) {
            mat.data[i][i] = 1.0f;
        }

        return mat;
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();

        for (int i = 0; i < size; i++) {
            sb.append("| ");

            for (int j = 0; j < size; j++) {
                float nCell = (data[i][j] == -0.0f) ? 0 : data[i][j];
                String sCell = String.format("%.1f", nCell);
                int rest = 8 - sCell.length();

                for(int k = 1; k < rest; k++) {
                    sb.append(" ");
                }

                sb.append(sCell);
            }

            sb.append("   |\n");
        }

        return sb.toString();
    }
}
