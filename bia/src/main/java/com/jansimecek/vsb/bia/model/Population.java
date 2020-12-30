package com.jansimecek.vsb.bia.model;

import org.jzy3d.maths.Coord3d;

import java.util.Collections;
import java.util.List;

public class Population {
    private Coord3d minimum;
    private List<Coord3d> points;

    public Population() {
        this.minimum = null;
        this.points = Collections.emptyList();
    }

    public Population(Coord3d minimum, List<Coord3d> points) {
        this.minimum = minimum;
        this.points = points;
    }

    public Coord3d getMinimum() {
        return minimum;
    }

    public void setMinimum(Coord3d minimum) {
        this.minimum = minimum;
    }

    public List<Coord3d> getPoints() {
        return points;
    }

    public void setPoints(List<Coord3d> points) {
        this.points = points;
    }
}
