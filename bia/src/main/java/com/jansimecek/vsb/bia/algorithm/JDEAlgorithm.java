package com.jansimecek.vsb.bia.algorithm;

import com.jansimecek.vsb.bia.function.IFunction;
import com.jansimecek.vsb.bia.model.JDEIndividual;
import com.jansimecek.vsb.bia.model.Population;
import org.jzy3d.maths.Coord3d;

import java.util.ArrayList;
import java.util.List;

public class JDEAlgorithm extends Algorithm {
    private float Fl, Fu, tau;
    private List<JDEIndividual> population, newPopulation;

    public JDEAlgorithm(IFunction function, int popSize, int generations, float tau) {
        super(function, popSize, generations);
        this.tau = tau;
        this.Fl = 0.1f;
        this.Fu = 1.0f;

        // Generate population
        List<Coord3d> points = generatePopulation();
        newPopulation = new ArrayList<>();
        population = new ArrayList<>();

        // Generate JDEAlgorithm population with random params
        for (Coord3d point : points) {
            JDEIndividual p = new JDEIndividual();
            p.p = point;
            p.F = rMinMax(Fl, Fu);
            p.CR = random.nextFloat();
            newPopulation.add(p);
        }
    }

    @Override
    public Population progress() {
        // Clear vector before generating new population
        population.clear();
        population.addAll(newPopulation);
        newPopulation.clear();

        // Generate new individuals
        for (JDEIndividual individual : population) {
            JDEIndividual[] individuals = new JDEIndividual[2], newIndividuals = new JDEIndividual[2];

            // Add default and one self-adapted individual
            individuals[0] = individual;
            individuals[1] = getSelfAdaptedIndividual(individual);

            for (int i = 0; i < 2; i++) {
                newIndividuals[i] = new JDEIndividual(individuals[i].F, individuals[i].CR);
                JDEIndividual[] solutionV = getSolutionVector(individuals[i]);

                // Compute [MUTATION]
                JDEIndividual noise = v(solutionV[0], solutionV[1], solutionV[2]);
                float r = random.nextFloat();

                // Assign coordinates [CROSSOVER]
                newIndividuals[i].p.x = (r < individuals[i].CR) ? noise.p.x : individuals[i].p.x;
                newIndividuals[i].p.y = noise.p.y; // Assign directly so there's always mutation
                newIndividuals[i].p.z = function.calcZ(newIndividuals[i].p.x, newIndividuals[i].p.y);
            }

            // Pick better one from the two new individuals
            JDEIndividual nInd;
            if (newIndividuals[0].p.z < newIndividuals[1].p.z) {
                nInd = newIndividuals[0];
            } else {
                nInd = newIndividuals[1];
            }

            // CHeck for fitness and bounds for new individual
            if (individual.p.z < nInd.p.z || !inBounds(nInd.p.getXY())) {
                newPopulation.add(individual);
            } else {
                newPopulation.add(nInd);
            }
        }

        // Find minimum in new population
        minimum = findMinimum(minimum, getPopulation(newPopulation));

        generation++;
        return new Population(minimum , getPopulation(newPopulation));
    }

    private JDEIndividual[] getSolutionVector(JDEIndividual individual) {
        JDEIndividual[] solution = new JDEIndividual[3];

        for (int i = 0; i < 3; i++) {
            JDEIndividual xN = null;
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
                for (JDEIndividual solutionP : solution) {
                    if (solutionP != null && solutionP.p !=null && solutionP.p.equals(xN.p)) {
                        same = true;
                        break;
                    }
                }
            }

            solution[i] = xN;
        }

        return solution;
    }

    private JDEIndividual v(JDEIndividual x1, JDEIndividual x2, JDEIndividual x3) {
        return new JDEIndividual(
            x1.p.x + x1.F * (x2.p.x - x3.p.x),
            x1.p.y + x1.F * (x2.p.y - x3.p.y),
            0, x1.F, x1.CR
        );
    }

    private JDEIndividual getSelfAdaptedIndividual(JDEIndividual individual) {
        float r1 = random.nextFloat(), r2 = random.nextFloat(), r3 = random.nextFloat();
        JDEIndividual newIndividual = new JDEIndividual();

        // Self-adapt
        newIndividual.p = new Coord3d(individual.p.x, individual.p.y, individual.p.z);
        newIndividual.F = (r1 < tau) ? rMinMax(Fl, Fu) : individual.F; // Fl + r1 * Fu
        newIndividual.CR = (r2 < tau) ? r3 : individual.CR;

        return newIndividual;
    }

    private List<Coord3d> getPopulation(List<JDEIndividual> individuals) {
        List<Coord3d> population = new ArrayList<>();

        for (JDEIndividual individual : individuals) {
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
