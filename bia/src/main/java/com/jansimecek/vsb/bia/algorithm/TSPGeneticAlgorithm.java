package com.jansimecek.vsb.bia.algorithm;

import com.jansimecek.vsb.bia.generators.TourManager;
import com.jansimecek.vsb.bia.model.City;
import com.jansimecek.vsb.bia.model.TSPIndividual;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Random;

public class TSPGeneticAlgorithm {
    private Random random;
    private int numGenerations, popSize, generation;
    private List<TSPIndividual> population, newPopulation;

    public TSPGeneticAlgorithm(int popSize, int numGenerations, List<City> cities) {
        this.generation = 0;
        this.popSize = popSize;
        this.numGenerations = numGenerations;
        this.random = new Random();

        // Generate population
        TourManager tm = new TourManager(cities);
        this.newPopulation = tm.generatePaths(popSize);
        this.population = new ArrayList<>();
    }

    public TSPIndividual progress() {
        // Clear vector before generating new population
        population.clear();
        population.addAll(newPopulation);
        newPopulation.clear();

        // Sort and pick best first half
        List<TSPIndividual> bestHalf = new ArrayList<>();
        bestHalf.addAll(sortByBestFitness(population));
        bestHalf.subList(bestHalf.size() / 2, bestHalf.size()).clear();

        for (TSPIndividual individual : population) {
            // Pick random parent
            final int N = individual.cities.size();
            TSPIndividual parent = bestHalf.get(random.nextInt(bestHalf.size()));
            int start = random.nextInt(N), end = random.nextInt(N);

            // Switch if start is larger than end
            if (end < start) {
                int tmp = start;
                start = end;
                end = tmp;
            }

            // Generate new individual
            TSPIndividual newIndividual = crossover(individual, parent, start, end);
            newIndividual.mutate(random.nextInt(N), random.nextInt(N));

            // Add better individual to populatino
            newPopulation.add((newIndividual.fitness() < individual.fitness()) ? newIndividual : individual);
        }

        generation++;
        return findMinimum(newPopulation);
    }

    private List<TSPIndividual> sortByBestFitness(List<TSPIndividual> population) {
        population.sort((i1, i2) -> Float.compare(i1.fitness(), i2.fitness()));
        return population;
    }

    private TSPIndividual findMinimum(List<TSPIndividual> population) {
        return sortByBestFitness(population).get(0);
    }

    private TSPIndividual crossover(TSPIndividual i1, TSPIndividual i2, int start, int end) {
        TSPIndividual nI = new TSPIndividual(i1.cities);

        for (int i = 0; i < nI.cities.size(); i++) {
            if (i >= start && i <= end) {
                // Replace with cities from i2 inside this range
                for (int j = 0; j < i2.cities.size(); j++) {
                    City currentCity = i2.cities.get(j);

                    // Check if this city is not in the new individual
                    boolean replace = true;
                    for (int k = 0; k < nI.cities.size(); k++) {
                        if (k >= i && k <= end) { continue; }
                        if (currentCity.equals(nI.cities.get(k))) {
                            replace = false;
                            break;
                        }
                    }

                    if (replace) {
                        nI.cities.set(i, currentCity);
                        break;
                    }
                }
            }
        }

        return nI;
    }

    public boolean isComplete() {
        return generation >= numGenerations;
    }

    public String info() {
        return "Generation: " + generation + "\n" +
                "Fitness: " + findMinimum(population).fitness();
    }
}
