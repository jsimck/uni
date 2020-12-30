package com.jansimecek.vsb.bia.algorithm;

import com.jansimecek.vsb.bia.function.IFunction;
import com.jansimecek.vsb.bia.model.JDEIndividual;
import com.jansimecek.vsb.bia.model.Population;
import org.jzy3d.maths.Coord3d;

import java.util.ArrayList;
import java.util.List;

public class EvolutionStrategy extends Algorithm {
    private float deviation, mutationIntensity;
    private final boolean isParentAndDescendant;
    private List<Coord3d> population, newPopulation;

    public EvolutionStrategy(IFunction function, int popSize, int numGenerations, float deviation, float mutationIntensity, boolean isParentAndDescendant) {
        super(function, popSize, numGenerations);
        this.deviation = deviation;
        this.isParentAndDescendant = isParentAndDescendant;
        this.mutationIntensity = mutationIntensity;

        // Generate population
        newPopulation = generatePopulation();
        population = new ArrayList<>();
    }

    @Override
    public Population progress() {
        // Clear vector before generating new population
        population.clear();
        population.addAll(newPopulation);
        newPopulation.clear();
        int successMutations = 0;

        // Generate new individuals
        for (Coord3d individual : population) {
            // Mutate individual
            Coord3d mutatedIndividual = new Coord3d();
            mutatedIndividual.x = individual.x + ((float) random.nextGaussian()) * deviation;
            mutatedIndividual.y = individual.y + ((float) random.nextGaussian()) * deviation;
            mutatedIndividual.z = function.calcZ(mutatedIndividual.x, mutatedIndividual.y);

            // Check if in bounds
            if (!inBounds(mutatedIndividual.getXY())) {
                mutatedIndividual = clampWithinBounds(mutatedIndividual);
            }

            if (isParentAndDescendant) {
                if (mutatedIndividual.z < individual.z) {
                    newPopulation.add(mutatedIndividual);
                    successMutations++;
                } else {
                    newPopulation.add(individual);
                }
            } else {
                successMutations++;
                newPopulation.add(mutatedIndividual);
            }
        }

        // Adjust deviation
        if (successMutations / 5 < population.size()) {
            deviation *= mutationIntensity;
        } else {
            deviation /= mutationIntensity;
        }

        // Find minimum in new population
        minimum = findMinimum(minimum, newPopulation);

        generation++;
        return new Population(minimum , newPopulation);
    }

    @Override
    public String info() {
        return "Generation: " + generation + "\n" +
                "Deviation: " + deviation + "\n" +
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
