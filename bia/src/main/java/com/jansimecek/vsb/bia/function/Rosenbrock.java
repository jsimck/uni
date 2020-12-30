package com.jansimecek.vsb.bia.function;

import com.jansimecek.vsb.bia.generators.PopulationGenerator;
import com.jansimecek.vsb.bia.model.GraphParams;

public class Rosenbrock implements IFunction {
    @Override
    public float calc(float... params) {
        float result = 0.0f;

        for (int i = 0; i < params.length - 1; i++) {
            result += 100.0f * (Math.pow(params[i + 1] - Math.pow(params[i], 2), 2) + Math.pow(params[i] - 1, 2));
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
        params.xMin = -2.0f;
        params.xMax = 2.0f;
        params.yMin = -1.0f;
        params.yMax = 3.0f;
        params.zMax = 3000.0f;

        return params;
    }

    @Override
    public float getGlobalMinimum() {
        return 0;
    }
}
