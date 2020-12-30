package com.jansimecek.vsb.bia.model;

import com.jansimecek.vsb.bia.utils.Utils;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

public class TSPIndividual {
    public List<City> cities;

    public TSPIndividual() {
        this.cities = new ArrayList<>();
    }

    public TSPIndividual(List<City> cities) {
        this();
        this.cities.addAll(cities);
    }

    public void mutate(int index1, int index2) {
        City tmp = cities.get(index1);
        cities.set(index1, cities.get(index2));
        cities.set(index2, tmp);
    }

    public float fitness() {
        float sum = 0;

        for (int i = 0; i < cities.size() - 1; i++) {
            sum += distance(cities.get(i), cities.get(i + 1));
        }

        // Calculate from last to first city too
        sum += distance(cities.get(cities.size() - 1), cities.get(0));

        return sum;
    }

    private float distance(City c1, City c2) {
        return (float) Math.sqrt(Utils.square(c2.x - c1.x) + Utils.square(c2.y - c1.y));
    }

    @Override
    public String toString() {
        return "[" + cities.stream()
                .map(i -> (i.name + " (" + i.x + ", " + i.y + ")"))
                .collect(Collectors.joining(", ")) + "]";
    }

    @Override
    public boolean equals(Object obj) {
        if (obj == null) {
            return false;
        } else if (!(obj instanceof TSPIndividual)) {
            return false;
        } else {
            TSPIndividual other = (TSPIndividual) obj;

            if (other.cities.size() != cities.size()) {
                return false;
            }

            for (int i = 0; i < cities.size(); i++) {
                if (!cities.get(i).equals(other.cities.get(i))) {
                    return false;
                }
            }

            return true;
        }
    }
}
