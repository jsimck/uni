package com.jansimecek.vsb.bia.algorithm;

import com.jansimecek.vsb.bia.function.IFunction;
import com.jansimecek.vsb.bia.model.Population;
import org.jzy3d.maths.Coord3d;

import java.util.List;

public class StochasticHillClimbing extends Algorithm {
    private float radius, alpha;

    public StochasticHillClimbing(IFunction function, int popSize, int generations, float radius, float alpha) {
        super(function, popSize, generations);
        this.radius = radius;
        this.alpha = alpha;

        // Generate minimum
        this.minimum = randomPointWithinBound();
    }

    @Override
    public Population progress() {
        List<Coord3d> points = generatePopulationRadial(minimum, radius);

        // Find minimum
        minimum = findMinimum(minimum, points);

        // Reduce radius
        radius *= alpha;

        // Return population
        generation++;
        return new Population(minimum, points);
    }

    @Override
    public boolean isComplete() {
        return minimumFoundOrFinished();
    }

    @Override
    public String info() {
        return "Generation: " + generation + "\n" +
                "Minimum:\n" +
                "  x: " + minimum.x + "\n" +
                "  y: " + minimum.y + "\n" +
                "  z: " + minimum.z + "\n" +
                "Alpha: " + alpha + "\n" +
                "Radius: " + radius;
    }

    @Override
    public String csv() {
        return generation + ";" + excelDecimalFormat(minimum.z) + ";" + excelDecimalFormat(alpha) + ";" + excelDecimalFormat(radius) + "\n";
    }
}
