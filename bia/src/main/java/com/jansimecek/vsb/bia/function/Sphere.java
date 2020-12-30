package com.jansimecek.vsb.bia.function;

import com.jansimecek.vsb.bia.model.GraphParams;
import com.jansimecek.vsb.bia.generators.PopulationGenerator;

public class Sphere implements IFunction {
    @Override
    public float calc(float... params) {
        float result = 0.0f;

        for (float x : params) {
            result += (x * x);
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
        params.xMin = params.yMin = -2.0f;
        params.xMax = params.yMax = 2.0f;
        params.zMax = 8.0f;

        return params;
    }

    @Override
    public float getGlobalMinimum() {
        return 0;
    }
}
