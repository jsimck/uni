package com.jansimecek.vsb.bia.function;

import com.jansimecek.vsb.bia.generators.PopulationGenerator;
import com.jansimecek.vsb.bia.model.GraphParams;

import java.security.InvalidParameterException;

public class Booth implements IFunction {
    @Override
    public float calc(float... params) {
        if (params.length > 2) {
            throw new InvalidParameterException("Bad parameter count: " + params.length + ", should be 2");
        }

        float x = params[0];
        float y = params[1];

        return (float) (Math.pow(x + 2 * y + 7, 2) + Math.pow(2 * x + y + 5, 2));
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
        params.zMax = 2500.0f;

        return params;
    }

    @Override
    public float getGlobalMinimum() {
        return 0;
    }
}
