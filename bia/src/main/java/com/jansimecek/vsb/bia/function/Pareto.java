package com.jansimecek.vsb.bia.function;

import com.jansimecek.vsb.bia.generators.PopulationGenerator;
import com.jansimecek.vsb.bia.model.GraphParams;

import java.security.InvalidParameterException;

public class Pareto implements IFunction {
    static final float G1 = 11;
    static final float G2 = 12;
    private float F;

    public Pareto() {
        this(1.0f);
    }

    public Pareto(float F) {
        this.F = F;
    }

    @Override
    public float calc(float... params) {
        if (params.length > 2) {
            throw new InvalidParameterException("Bad parameter count: " + params.length + ", should be 2");
        }

        float x1 = params[0];
        float x2 = params[1];

        return (float) (Math.pow(f(x1) / g(x2), alpha(x2)) - (f(x1) / g(x2) * Math.sin(Math.PI * F * f(x1) * g(x2))));
    }

    @Override
    public float calcZ(float x, float y) {
        return calc(x, y, F);
    }

    @Override
    public GraphParams getParams() {
        GraphParams params = new GraphParams();
        params.xMin = params.yMin = 0.0f;
        params.xMax = params.yMax = 1.0f;
        params.zMax = .2f;
        params.zMin = -.2f;
        params.zAxisLabel = "f1";
        params.xAxisLabel = "g";

        return params;
    }

    private static float f(float x) {
        return x;
    }

    private static float g(float x) {
        return x + 10.0f;
    }

    private static float alpha(float x) {
        return 0.25f + 3.75f * ((g(x) - G2) / (G1 - G2));
    }

    @Override
    public float getGlobalMinimum() {
        return 0;
    }
}
