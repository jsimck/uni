package com.jansimecek.vsb.bia.model;

public interface IGraphModel {
    public GraphParams getParams();
    public float calculateZ(float x, float y);
}
