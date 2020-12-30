package com.jansimecek.vsb.bia.function;

import com.jansimecek.vsb.bia.model.GraphParams;

public interface IFunction {
    public float calc(float ... params);
    public float calcZ(float x, float y);
    public GraphParams getParams();
    public float getGlobalMinimum();
}
