package com.jansimecek.vsb.bia.function;

import com.jansimecek.vsb.bia.model.GraphParams;
import com.jansimecek.vsb.bia.generators.PopulationGenerator;

public class Rastrigin implements IFunction {
    private float A;

    public Rastrigin() {
        this(10.0f);
    }

    public Rastrigin(float A) {
        this.A = A;
    }

    @Override
    public float calc(float... params) {
        float result = A * (float) params.length;

        for (float x : params) {
            result += (x * x) - A * Math.cos(2.0f * Math.PI * x);
        }

        return result;
    }

    @Override
    public float calcZ(float x, float y) {
        return calc(x, y);
    }

    @Override
    public GraphParams getParams() {
        GraphParams params = new GraphParams();
        params.xMin = params.yMin = -5.0f;
        params.xMax = params.yMax = 5.0f;
        params.zMax = 80.0f;

        return params;
    }

    @Override
    public float getGlobalMinimum() {
        return 0;
    }
}
