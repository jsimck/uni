package com.jansimecek.vsb.bia.algorithm;

import com.jansimecek.vsb.bia.function.IFunction;
import com.jansimecek.vsb.bia.model.Population;
import org.jzy3d.maths.Coord3d;

import java.util.List;

public class BlindAlgorithm extends Algorithm {
    public BlindAlgorithm(IFunction function, int popSize, int generations) {
        super(function, popSize, generations);
    }

    @Override
    public Population progress() {
        List<Coord3d> points = generatePopulation();

        // Find minimum
        minimum = findMinimum(minimum, points);

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
                "  z: " + minimum.z;
    }

    @Override
    public String csv() {
        return generation + ";" + excelDecimalFormat(minimum.z) + "\n";
    }
}