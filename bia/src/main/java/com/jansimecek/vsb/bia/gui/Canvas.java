package com.jansimecek.vsb.bia.gui;

import com.jansimecek.vsb.bia.model.GraphParams;
import com.jansimecek.vsb.bia.function.IFunction;
import org.jzy3d.chart.Chart;
import org.jzy3d.chart.controllers.mouse.camera.AWTCameraMouseController;
import org.jzy3d.chart.factories.AWTChartComponentFactory;
import org.jzy3d.colors.Color;
import org.jzy3d.colors.ColorMapper;
import org.jzy3d.colors.colormaps.*;
import org.jzy3d.maths.Coord3d;
import org.jzy3d.maths.Range;
import org.jzy3d.plot3d.builder.Builder;
import org.jzy3d.plot3d.builder.Mapper;
import org.jzy3d.plot3d.builder.concrete.OrthonormalGrid;
import org.jzy3d.plot3d.primitives.Scatter;
import org.jzy3d.plot3d.primitives.Shape;
import org.jzy3d.plot3d.rendering.canvas.Quality;

import javax.swing.*;
import java.awt.Component;
import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.io.File;
import java.io.IOException;
import java.util.List;

public class  Canvas extends JPanel {
    private final int steps = 120;

    private IFunction function;
    private GraphParams params;
    private JPanel infoPanel;
    private Component canvas;
    private Chart chart;
    private Scatter scatter, scatterMin, scatterPopInt, scatterPopFloat;
    private Shape surface;

    public Canvas(IFunction function) {
        this.function = function;
        this.params = function.getParams();

        // Init GUI
        init();
    }

    public void update(IFunction function, GraphParams params) {
        // Save new params
        this.params = params;

        // Redraw surface
        if (function != this.function) {
            this.function = function;
            chart.getScene().getGraph().remove(this.surface);
            this.surface = initSurface();
            chart.getScene().getGraph().add(this.surface);
        }

        // Remove scatters
        if (scatterPopFloat != null) {
            chart.getScene().remove(scatterPopFloat);
        }

        if (scatterPopInt != null) {
            chart.getScene().remove(scatterPopInt);
        }

        if (scatter != null) {
            chart.getScene().remove(scatter);
        }

        if (scatterMin != null) {
            chart.getScene().remove(scatterMin);
        }

        // Draw population
        if (params.populationFloat != null) {
            scatterPopFloat = drawPopulation(params.populationFloat, params.populationFloatColor, 15);
            chart.getScene().add(scatterPopFloat);
        }

        if (params.populationInt != null) {
            scatterPopInt = drawPopulation(params.populationInt, params.populationIntColor, 15);
            chart.getScene().add(scatterPopInt);
        }

        if (params.population != null) {
            scatter = drawAlgPopulation(params.population.getPoints(), params.populationColor, 15);
            chart.getScene().add(scatter);
        }

        if (params.population != null) {
            scatterMin = drawPoint(params.population.getMinimum(), params.populationMinimumColor, 30);
            chart.getScene().add(scatterMin);
        }

        this.revalidate();
    }

    private Scatter drawPoint(Coord3d point, Color color, float size) {
        Coord3d[] points = new Coord3d[1];
        points[0] = point;
        return new Scatter(points, color, size);
    }

    private Scatter drawPopulation(List<Coord3d> population, Color color, float size) {
        Coord3d[] points = population.toArray(new Coord3d[population.size()]);
        return new Scatter(points, color, size);
    }

    private Scatter drawAlgPopulation(List<Coord3d> population, Color color, float size) {
        Coord3d[] points = population.toArray(new Coord3d[population.size()]);

        if (color != null) {
            return new Scatter(points, color, size);
        } else {
            // Find minimum and maximum
            float zMin = points[0].z, zMax = points[0].z;
            for (Coord3d point : points) {
                if (point.z < zMin) { zMin = point.z;}
                if (point.z >= zMax) { zMax = point.z; }
            }

            // Init colors
            ColorMapper mapper = new ColorMapper(new ColorMapWhiteRed(), zMin, zMax);
            Color[] colors = new Color[points.length];
            for (int i = 0; i < colors.length; i++) {
                colors[i] = mapper.getColor(points[i]);
            }

            return new Scatter(points, colors, size);
        }
    }

    private Shape initSurface() {
        Mapper mapper = new Mapper() {
            @Override
            public double f(double x, double y) {
                return function.calcZ((float) x, (float) y);
            }
        };

        // Draw canvas
        Range rangeX = new Range(params.xMin, params.xMax);
        Range rangeY = new Range(params.yMin, params.yMax);

        // Create the object to represent the function over the given range.
        Shape surface = Builder.buildOrthonormal(new OrthonormalGrid(rangeX, steps, rangeY, steps), mapper);
        surface.setColorMapper(new ColorMapper(new ColorMapRainbow(), surface.getBounds().getZmin(), surface.getBounds().getZmax(), new org.jzy3d.colors.Color(1, 1, 1, 0.4f)));
        surface.setWireframeDisplayed(params.wireframe);
        surface.setFaceDisplayed(true);

        return surface;
    }

    private void init() {
        this.setLayout(new BorderLayout());

        // Info panel
//        infoPanel = new JPanel(new FlowLayout(FlowLayout.CENTER, 50, 5));
//        infoPanel.setBackground(new java.awt.Color(190, 190, 190));
//        infoPanel.add(new JLabel("Rotate: Mouse Click & Drag"));
//        infoPanel.add(new JLabel("Adjust Z-range: Scroll"));
//        infoPanel.add(new JLabel("Move Z: Shift Key + Mouse Click & Drag"));

        // Create a chart
        chart = AWTChartComponentFactory.chart(Quality.Advanced);
        chart.getAxeLayout().setXAxeLabel(params.xAxisLabel);
        chart.getAxeLayout().setYAxeLabel(params.yAxisLabel);
        chart.getAxeLayout().setZAxeLabel(params.zAxisLabel);
        this.surface = initSurface();
        chart.getScene().getGraph().add(surface);

        // Get canvas component and append it into layout
        AWTCameraMouseController controller = new AWTCameraMouseController(chart);
        canvas = (Component) chart.getCanvas();
        canvas.addMouseListener(controller);
        canvas.addMouseMotionListener(controller);
        canvas.addMouseWheelListener(controller);
        canvas.setPreferredSize(new Dimension(1100, 850));

        // Add to container
        this.add(canvas, BorderLayout.CENTER);

        // Add to container
//        this.add(infoPanel, BorderLayout.SOUTH);
    }

    public void screenShot(File file) throws IOException {
        chart.screenshot(file);
    }
}
