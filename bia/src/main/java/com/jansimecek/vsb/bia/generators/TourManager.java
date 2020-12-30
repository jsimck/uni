package com.jansimecek.vsb.bia.generators;

import com.jansimecek.vsb.bia.model.City;
import com.jansimecek.vsb.bia.model.SPoint;
import com.jansimecek.vsb.bia.model.TSPIndividual;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class TourManager {
    private List<City> cities;

    public TourManager() {
        cities = new ArrayList<>();
    }

    public TourManager(List<City> cities) {
        this();
        this.cities.addAll(cities);
    }

    public void addCity(City city) {
        cities.add(city);
    }

    public void addCities(List<City> city) {
        cities.addAll(city);
    }

    public List<TSPIndividual> generatePaths(int count) {
        List<TSPIndividual> individuals = new ArrayList<>();
        List<City> permutations = new ArrayList<>();
        permutations.addAll(cities);

        for (int i = 0; i < count; i++) {
            TSPIndividual newIndividual;
            boolean same = false;

            do {
                Collections.shuffle(permutations);
                newIndividual = new TSPIndividual(permutations);

                for (TSPIndividual tspIndividual : individuals) {
                    if (tspIndividual.equals(newIndividual)) {
                        same = true;
                        break;
                    }
                }
            } while (same);

            individuals.add(newIndividual);
        }

        return individuals;
    }
}
