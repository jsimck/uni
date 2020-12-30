package com.jansimecek.vsb.bia.model;

import org.jzy3d.maths.Coord3d;

import org.jzy3d.colors.*;
import java.util.Collections;
import java.util.List;

public final class GraphParams {
    public boolean wireframe =  false;
    public float xMin = -5.0f;
    public float xMax = 5.0f;
    public float yMin = -5.0f;
    public float yMax = 5.0f;
    public float zMin = 0;
    public float zMax = 10.0f;
    public String xAxisLabel = "X";
    public String yAxisLabel = "Y";
    public String zAxisLabel = "Z";

    // Custom colors
    public Color populationIntColor = Color.BLUE;
    public Color populationFloatColor = Color.RED;
    public Color populationMinimumColor = Color.GREEN;
    public Color populationColor = null;

    // Population
    public List<Coord3d> populationInt = Collections.emptyList();
    public List<Coord3d> populationFloat = Collections.emptyList();
    public Population population = new Population();
}
