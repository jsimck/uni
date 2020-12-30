package com.jansimecek.vsb.bia.algorithm;

import com.jansimecek.vsb.bia.function.IFunction;
import com.jansimecek.vsb.bia.model.JDEIndividual;
import com.jansimecek.vsb.bia.model.PSIndividual;
import com.jansimecek.vsb.bia.model.Population;
import org.jzy3d.maths.Coord3d;

import java.util.ArrayList;
import java.util.List;

public class ParticleSwarmAlgorithm extends Algorithm {
    private float vMax, c1, c2;
    private List<PSIndividual> population, newPopulation;

    public ParticleSwarmAlgorithm(IFunction function, int popSize, int numGenerations, float vMax, float c1, float c2) {
        super(function, popSize, numGenerations);
        this.vMax = vMax;
        this.c1 = c1;
        this.c2 = c2;

        // Generate population
        List<Coord3d> points = generatePopulation();
        newPopulation = new ArrayList<>();
        population = new ArrayList<>();

        // Generate JDEAlgorithm population with random params
        for (Coord3d point : points) {
            PSIndividual p = new PSIndividual(point, rMinMax(0, vMax), rMinMax(0, vMax));
            newPopulation.add(p);
        }

        // Find minimum (pBest)
        minimum = findMinimum(minimum, getPopulation(newPopulation));
    }

    @Override
    public Population progress() {
        // Clear vector before generating new population
        population.clear();
        population.addAll(newPopulation);
        newPopulation.clear();

        // Generate new individuals
        for (PSIndividual individual : population) {
            // Generate velocity vector
            individual.v0 = computeVelocity(individual.v0, individual.p.x, individual.pBest.x, minimum.x);
            individual.v1 = computeVelocity(individual.v1, individual.p.y, individual.pBest.y, minimum.y);

            // Adjust speed
            individual.p.x = individual.v0 + individual.p.x;
            individual.p.y = individual.v1 + individual.p.y;
            individual.p.z = function.calcZ(individual.p.x, individual.p.y);

            // Check if it's within bounds
            if (!inBounds(individual.p.getXY())) {
                individual.p = clampWithinBounds(individual.p);
            }

            // Save pBest
            if (individual.p.z < individual.pBest.z) {
                individual.pBest = individual.p.clone();
            }

            // Save gBest
            if (individual.p.z < minimum.z) {
                minimum = individual.p.clone();
            }

            newPopulation.add(individual);
        }

        generation++;
        return new Population(minimum, getPopulation(newPopulation));
    }

    private float computeVelocity(float vi, float xi, float pBest, float gBest) {
        return 0.95f * vi + (c1 * random.nextFloat()) * (pBest - xi) + (c2 * random.nextFloat()) * (gBest - xi);
    }

    private List<Coord3d> getPopulation(List<PSIndividual> individuals) {
        List<Coord3d> population = new ArrayList<>();

        for (PSIndividual individual : individuals) {
            population.add(individual.p);
        }

        return population;
    }

    @Override
    public String info() {
        return "Generation: " + generation + "\n" +
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

