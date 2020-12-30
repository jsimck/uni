package com.jansimecek.vsb.bia.function;

import com.jansimecek.vsb.bia.generators.PopulationGenerator;
import com.jansimecek.vsb.bia.model.GraphParams;

import java.security.InvalidParameterException;

public class BukinN6 implements IFunction {
    @Override
    public float calc(float... params) {
        if (params.length > 2) {
            throw new InvalidParameterException("Bad parameter count: " + params.length + ", should be 2");
        }

        float x = params[0];
        float y = params[1];

        return 100.0f * (float) Math.sqrt(Math.abs(y - (0.01f * x * x))) + 0.01f * Math.abs(x + 10.0f);
    }

    @Override
    public float calcZ(float x, float y) {
        return calc(x, y);
    }

    @Override
    public GraphParams getParams() {
        GraphParams params = new GraphParams();
        params.xMin = -15.0f;
        params.xMax = -5.0f;
        params.yMin = -3.0f;
        params.yMax = 3.0f;
        params.zMax = 220.0f;

        return params;
    }

    @Override
    public float getGlobalMinimum() {
        return 0;
    }
}
