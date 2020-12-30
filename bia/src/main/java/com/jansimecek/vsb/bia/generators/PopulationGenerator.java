package com.jansimecek.vsb.bia.generators;

import com.jansimecek.vsb.bia.function.IFunction;
import com.jansimecek.vsb.bia.model.GraphParams;
import org.jzy3d.maths.Coord3d;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class PopulationGenerator {
    public static final int TYPE_FLOAT = 0;
    public static final int TYPE_INT = 1;

    private static final long currentTime = System.currentTimeMillis();
    private static final Random r = new Random(currentTime);

    public List<Coord3d> populate(IFunction function, int type, int count, float xMin, float yMin, float xMax, float yMax) {
        List<Coord3d> points = new ArrayList<>();

        for (int i = 0; i < count; i++) {
            float x = xMin + r.nextFloat() * (xMax - xMin);
            float y = yMin + r.nextFloat() * (yMax - yMin);

            // Cast to int
            if (type == TYPE_INT) {
                x = (int) x;
                y = (int) y;
            }

            float z = function.calcZ(x, y);
            points.add(new Coord3d(x, y, z));
        }

        return points;
    }

    public List<Coord3d> populateRadial(IFunction function, int type, int count, float xMin, float yMin, float xMax, float yMax, Coord3d center, float radius) {
        List<Coord3d> points = new ArrayList<>();

        for (int i = 0; i < count; i++) {
            // Generate random angle in radians between 0 and 2PI
            double angle = (r.nextDouble() * (2 * Math.PI));
            float radiusX = xMin + ((xMax - xMin) * r.nextFloat());
            float radiusY = yMin + ((yMax - yMin) * r.nextFloat());

            // Get x and y
            float x = center.x + (float) ((radiusX * radius) * Math.cos(angle));
            float y = center.y + (float) ((radiusY * radius) * Math.sin(angle));

            // Cast to int
            if (type == TYPE_INT) {
                x = (int) x;
                y = (int) y;
            }

            float z = function.calcZ(x, y);
            points.add(new Coord3d(x, y, z));
        }

        return points;
    }
}
