package com.jansimecek.vsb.bia.function;

import com.jansimecek.vsb.bia.generators.PopulationGenerator;
import com.jansimecek.vsb.bia.model.GraphParams;

import java.security.InvalidParameterException;

public class Beale implements IFunction {
    @Override
    public float calc(float... params) {
        if (params.length > 2) {
            throw new InvalidParameterException("Bad parameter count: " + params.length + ", should be 2");
        }

        float x = params[0];
        float y = params[1];

        return (float) (Math.pow(1.5f - x + (x * y), 2) + Math.pow(2.25f - x + (x * Math.pow(y, 2)), 2) + Math.pow(2.625 - x + (x * Math.pow(y, 3)), 2));
    }

    @Override
    public float calcZ(float x, float y) {
        return calc(x, y);
    }

    @Override
    public GraphParams getParams() {
        GraphParams params = new GraphParams();
        params.yMin = params.xMin = -4.5f;
        params.yMax = params.xMax = 4.5f;
        params.zMax = 150000.0f;

        return params;
    }

    @Override
    public float getGlobalMinimum() {
        return 0;
    }
}
