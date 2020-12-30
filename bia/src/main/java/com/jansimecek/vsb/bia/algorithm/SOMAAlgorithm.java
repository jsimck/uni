package com.jansimecek.vsb.bia.algorithm;

import com.jansimecek.vsb.bia.function.IFunction;
import com.jansimecek.vsb.bia.model.Population;
import org.jzy3d.maths.Coord3d;

import java.util.ArrayList;
import java.util.List;

public class SOMAAlgorithm extends Algorithm {
    private float pathLength, step, PRT;
    private List<Coord3d> population;


    public SOMAAlgorithm(IFunction function, int popSize, int numGenerations, float pathLength, float step, float PRT) {
        super(function, popSize, numGenerations);
        this.pathLength = pathLength;
        this.step = step;
        this.PRT = PRT;

        // Generate population
        population = generatePopulation();
        minimum = findMinimum(minimum, population);
    }

    @Override
    public Population progress() {
        // Find best
        Coord3d king = findMinimum(minimum, population);
        List<Coord3d> newPopulation = new ArrayList<>();

        // Migrate all other individuals in direction of generated PTR vector
        for (Coord3d individual : population) {
            Coord3d currentBest = individual.clone();

            for (float t = step; t < pathLength; t += step) {
                int[] PRTVector = getPRTVector(2);
                Coord3d jumpedIndividual = new Coord3d();

                // Move individual by ptr vector
                jumpedIndividual.x = individual.x + (king.x - individual.x) * t * PRTVector[0];
                jumpedIndividual.y = individual.y + (king.y - individual.y) * t * PRTVector[1];
                jumpedIndividual.z = function.calcZ(jumpedIndividual.x, jumpedIndividual.y);

                // Save best results
                if (jumpedIndividual.z < currentBest.z) {
                    currentBest = jumpedIndividual;
                }
            }

            newPopulation.add(currentBest);
        }

        // Assign new population & find minimum in new population
        population = newPopulation;
        minimum = findMinimum(minimum, population);

        generation++;
        return new Population(minimum , population);
    }

    private int[] getPRTVector(int N) {
        int[] PRTVector = new int[N];

        for (int i = 0; i < N; i++) {
            PRTVector[i] = random.nextFloat() < PRT ? 1 : 0;
        }

        return PRTVector;
    }

    @Override
    public String info() {
        return "Migration: " + generation + "\n" +
                "Minimum:\n" +
                "  x: " + minimum.x + "\n" +
                "  y: " + minimum.y + "\n" +
                "  z: " + minimum.z + "\n";
    }

    @Override
    public String csv() {
        return generation + ";" + excelDecimalFormat(minimum.z) + "\n";
    }

    @Override
    public boolean isComplete() {
        return minimumFoundOrFinished();
    }
}
