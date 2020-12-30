package com.jansimecek.vsb.bia.function;

import com.jansimecek.vsb.bia.generators.PopulationGenerator;
import com.jansimecek.vsb.bia.model.GraphParams;

import java.security.InvalidParameterException;

public class CrossInTray implements IFunction {
    @Override
    public float calc(float... params) {
        if (params.length > 2) {
            throw new InvalidParameterException("Bad parameter count: " + params.length + ", should be 2");
        }

        float x = params[0];
        float y = params[1];

        return -0.00001f * (float) Math.pow(Math.abs(Math.sin(x) * Math.sin(y) * Math.exp(Math.abs(100.0 - (Math.sqrt(x * x + y * y)) / Math.PI))) + 1.0, 0.1);
    }

    @Override
    public float calcZ(float x, float y) {
        return calc(x, y);
    }

    @Override
    public GraphParams getParams() {
        GraphParams params = new GraphParams();
        params.xMin = params.yMin = -10.0f;
        params.xMax = params.yMax = 10.0f;
        params.zMax = 0.05f;
        params.zMin = -0.25f;

        return params;
    }

    @Override
    public float getGlobalMinimum() {
        return -2.06261f;
    }
}
