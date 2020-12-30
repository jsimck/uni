package com.jansimecek.vsb.bia.function;

import com.jansimecek.vsb.bia.generators.PopulationGenerator;
import com.jansimecek.vsb.bia.model.GraphParams;

import java.security.InvalidParameterException;

public class Ackley implements IFunction {
    @Override
    public float calc(float... params) {
        if (params.length > 2) {
            throw new InvalidParameterException("Bad parameter count: " + params.length + ", should be 2");
        }

        float x = params[0];
        float y = params[1];

        return (-20.0f * (float) Math.exp(-0.2f * Math.sqrt(0.5f * (x * x + y * y)))) -
                (float) (-Math.exp(0.5f * (Math.cos(2.0f * Math.PI * x + 2.0f * Math.PI * y)))) + (float) Math.E + 20.0f;
    }

    @Override
    public float calcZ(float x, float y) {
        return calc(x, y);
    }

    @Override
    public GraphParams getParams() {
        GraphParams params = new GraphParams();
        params.xMin = params.yMin = -4.5f;
        params.xMax = params.yMax = 4.5f;
        params.zMax = 16.0f;

        return params;
    }

    @Override
    public float getGlobalMinimum() {
        return 0;
    }
}
