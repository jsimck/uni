package com.jansimecek.vsb.bia.algorithm;

import com.jansimecek.vsb.bia.function.IFunction;
import com.jansimecek.vsb.bia.generators.PopulationGenerator;
import com.jansimecek.vsb.bia.model.GraphParams;
import com.jansimecek.vsb.bia.model.Population;
import org.jzy3d.maths.Coord2d;
import org.jzy3d.maths.Coord3d;

import java.math.BigDecimal;
import java.util.List;
import java.util.Random;

public abstract class Algorithm {
    // Algorithm params
    protected int generation;
    protected int numGenerations;
    protected int popSize;
    protected Coord3d minimum;
    protected IFunction function;
    protected GraphParams params;

    // Utils
    protected PopulationGenerator generator;
    protected Random random;

    public Algorithm(IFunction function, int popSize, int numGenerations) {
        this.function = function;
        this.popSize = popSize;
        this.params = function.getParams();
        this.minimum = new Coord3d(0, 0, params.zMax);
        this.generator = new PopulationGenerator();
        this.random = new Random();
        this.numGenerations = numGenerations;
        this.generation = 0;
    }

    protected List<Coord3d> generatePopulation() {
        return generator.populate(function, PopulationGenerator.TYPE_FLOAT, popSize,
                params.xMin, params.yMin, params.xMax, params.yMax);
    }

    protected List<Coord3d> generatePopulationRadial(Coord3d center, float radius) {
        return generator.populateRadial(function, PopulationGenerator.TYPE_FLOAT, popSize,
                params.xMin, params.yMin, params.xMax, params.yMax, center, radius);
    }

    protected Coord3d randomPointWithinBound() {
        float x = params.xMin + (random.nextFloat() * (params.xMax - params.xMin));
        float y = params.yMin + (random.nextFloat() * (params.yMax - params.yMin));
        float z = function.calcZ(x, y);

        return new Coord3d(x ,y, z);
    }

    protected Coord3d findMinimum(Coord3d min, List<Coord3d> points) {
        for (Coord3d p : points) {
            if (p.z < min.z) {
                min = p;
            }
        }

        return min;
    }

    protected String excelDecimalFormat(float value) {
        return (new BigDecimal(value).toPlainString()).replace('.', ',');
    }

    protected boolean minimumFoundOrFinished() {
        return generation >= numGenerations || minimum.z == function.getGlobalMinimum();
    }

    protected boolean inBounds(Coord2d point) {
        return point.x < params.xMax && point.x > params.xMin && point.y < params.yMax && point.y > params.yMin;
    }

    protected Coord3d clampWithinBounds(Coord3d point) {
        point.x = (point.x < params.xMin) ? params.xMin + 0.1f : point.x;
        point.y = (point.y < params.yMin) ? params.yMin + 0.1f : point.y;
        point.x = (point.x > params.xMax) ? params.xMax - 0.1f : point.x;
        point.y = (point.y > params.yMax) ? params.yMax - 0.1f : point.y;
        point.z = function.calcZ(point.x, point.y);

        return new Coord3d(point.x, point.y, point.z);
    }

    protected float rMinMax(float min, float max) {
        return min + random.nextFloat() * (max - min);
    }

    // Abstracts
    public abstract Population progress();
    public abstract String info();
    public abstract String csv();
    public abstract boolean isComplete();

    public int getPopSize() {
        return popSize;
    }

    public Coord3d getMinimum() {
        return minimum;
    }

    public IFunction getFunction() {
        return function;
    }

    public GraphParams getParams() {
        return params;
    }

    public int getGeneration() { return generation; }
}
