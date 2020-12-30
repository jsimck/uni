package com.jansimecek.vsb.bia.algorithm;

import com.jansimecek.vsb.bia.function.IFunction;
import com.jansimecek.vsb.bia.model.Population;
import org.jzy3d.maths.Coord3d;

import java.util.ArrayList;
import java.util.List;

public class DifferentialEvolution extends Algorithm {
    private float F, CR;
    private List<Coord3d> population, newPopulation;

    public DifferentialEvolution(IFunction function, int popSize, int generations, float F, float CR) {
        super(function, popSize, generations);
        this.F = F;
        this.CR = CR;

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

        // Generate new individuals
        for (Coord3d individual : population) {
            Coord3d newIndividual = new Coord3d();
            Coord3d[] solutionV = getSolutionVector(individual);

            // Compute
            Coord3d noise = v(solutionV[0], solutionV[1], solutionV[2]);
            float r = random.nextFloat();

            // Assign coordinates
            newIndividual.x = (r < CR) ? noise.x : individual.x;
            newIndividual.y = noise.y; // Assign directly so there's always mutation
            newIndividual.z = function.calcZ(newIndividual.x, newIndividual.y);

            // Push individual with better fitness to population
            if (individual.z < newIndividual.z || !inBounds(newIndividual.getXY())) {
                newPopulation.add(individual);
            } else {
                newPopulation.add(newIndividual);
            }
        }

        // Find minimum in new population
        minimum = findMinimum(minimum, newPopulation);

        generation++;
        return new Population(minimum , newPopulation);
    }

    private Coord3d[] getSolutionVector(Coord3d individual) {
        Coord3d[] solution = new Coord3d[3];

        for (int i = 0; i < 3; i++) {
            Coord3d xN = null;
            boolean same = true;

            // Find 3 unique random points
            while (same) {
                same = false;
                xN = population.get(random.nextInt(popSize));

                // Check if not same with src
                if (individual.equals(xN)) {
                    same = true;
                }

                // Check with other points
                for (Coord3d solutionP : solution) {
                    if (solutionP !=null && solutionP.equals(xN)) {
                        same = true;
                        break;
                    }
                }
            }

            solution[i] = xN;
        }

        return solution;
    }

    private Coord3d v(Coord3d x1, Coord3d x2, Coord3d x3) {
        return new Coord3d(
                x1.x + F * (x2.x - x3.x),
                x1.y + F * (x2.y - x3.y),
                0
        );
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
