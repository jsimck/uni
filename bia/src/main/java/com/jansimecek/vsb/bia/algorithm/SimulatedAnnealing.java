package com.jansimecek.vsb.bia.algorithm;

import com.jansimecek.vsb.bia.function.IFunction;
import com.jansimecek.vsb.bia.model.Population;
import org.jzy3d.maths.Coord3d;

import java.util.List;

public class SimulatedAnnealing extends Algorithm {
    // Alg params
    private float T0 = 0;
    private float TFinal = 0;
    private float alpha = 0;
    private float radius = 0;

    public SimulatedAnnealing(IFunction function, int popSize, int generations, float T0, float TFinal, float alpha, float radius) {
        super(function, popSize, generations);
        this.T0 = T0;
        this.TFinal = TFinal;
        this.alpha = alpha;
        this.radius = radius;

        // Random starting point
        this.minimum = randomPointWithinBound();
    }

    @Override
    public Population progress() {
        // Generate points in minimum surroundings
        List<Coord3d> points = generatePopulationRadial(minimum, radius);

        // Clamp population in function range
        for (Coord3d point : points) {
            if (!inBounds(point.getXY())) {
                point = clampWithinBounds(point);
            }
        }

        // Find minimum
        Coord3d nMin = new Coord3d(0, 0, params.zMax);
        nMin = findMinimum(nMin, points);

        // Annealing
        float fx = minimum.z - nMin.z;
        if (fx > 0) {
            minimum = nMin;
        } else {
            float r = random.nextFloat();
            if (r < Math.exp(fx / T0)) {
                minimum = nMin;
            }
        }

        // Decrease T0
        T0 *= alpha;

        generation++;
        return new Population(minimum, points);
    }

    @Override
    public boolean isComplete() {
        return minimumFoundOrFinished() || T0 <= TFinal;
    }

    @Override
    public String info() {
        return "Generation: " + generation + "\n" +
                "Minimum:\n" +
                "  x: " + minimum.x + "\n" +
                "  y: " + minimum.y + "\n" +
                "  z: " + minimum.z + "\n" +
                "Alpha: " + alpha + "\n" +
                "T0: " + T0;
    }

    @Override
    public String csv() {
        return generation + ";" + excelDecimalFormat(minimum.z) + ";" + excelDecimalFormat(alpha) + ";" + excelDecimalFormat(T0) + "\n";
    }
}
