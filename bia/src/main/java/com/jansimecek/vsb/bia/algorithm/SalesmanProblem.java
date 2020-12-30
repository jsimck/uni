package com.jansimecek.vsb.bia.algorithm;

import com.jansimecek.vsb.bia.model.SPoint;
import com.jansimecek.vsb.bia.utils.Utils;

import java.util.ArrayList;
import java.util.List;

public class SalesmanProblem {
    public void run() {
        // Possible trajectories
        for (int n = 3; n <= 15; n++) {
            System.out.println(Utils.factorial(n - 1));
        }

        // Possible trajectories
        final int min = 3, max = 15;
        for (int i = min; i <= max; i++) {
            long start = (i > 9) ? System.currentTimeMillis() : System.nanoTime();

            List<SPoint> points = new ArrayList<>();
            for (int k = 1; k < i; k++) {
                points.add(SPoint.generate(k));
            }

            permutatePoints(points, 0);
            points.add(0, SPoint.generate(0));
            System.out.println(points);

            long end = (i > 9) ? System.currentTimeMillis() : System.nanoTime();
            if (i > 9) {
                System.out.println(i + ". " + (end - start) + "ms");
            } else {
                System.out.println(i + ". " + ((double) (end - start) / 1000000.0) + "ms");
            }
        }
    }

    private void swap(List<SPoint> points, int index1, int index2) {
        SPoint tmpPoint = points.get(index1);
        points.set(index1, points.get(index2));
        points.set(index2, tmpPoint);
    }

    public void permutatePoints(List<SPoint> points, int index) {
        if (index == points.size()) {
            return;
        }

        permutatePoints(points, index + 1);

        for (int i = index + 1; i < points.size(); i++) {
            swap(points, i, index);
            permutatePoints(points, index + 1);
            swap(points, i, index);
        }
    }
}
