package com.jansimecek.vsb.bia.gui;

import com.jansimecek.vsb.bia.algorithm.Algorithm;
import com.jansimecek.vsb.bia.function.IFunction;
import com.jansimecek.vsb.bia.function.Sphere;
import com.jansimecek.vsb.bia.generators.PopulationGenerator;
import com.jansimecek.vsb.bia.model.GraphParams;
import org.jzy3d.maths.Coord3d;

import javax.swing.*;
import java.awt.*;
import java.io.*;
import java.security.spec.ECField;
import java.util.List;

public class MainWindow extends JFrame {
    private Canvas canvas;
    private Sidebar sidebar;
    private PopulationGenerator generator;
    private Thread drawingThread;
    private String basePath;

    public MainWindow() {
        super("BIA");

        generator = new PopulationGenerator();
        basePath = "/Users/jansimecek/desktop/";

        // Init gui
        init();
    }

    private void init() {
        JPanel panel = new JPanel();
        panel.setLayout(new BorderLayout());

        // Init components
        canvas = new Canvas(new Sphere());
        sidebar = new Sidebar();

        // Add sidebar listener
        sidebar.addSidebarActionListener((e, f, alg, continuous, popEnabled, popIntC, popFloatC, generations, screenShots, screenShotBreaks) -> {
            GraphParams params = f.getParams();
            String fileName = f.getClass().getSimpleName() + "_" + alg.getClass().getSimpleName();

            // Draw population
            if (popEnabled) {
                params.populationInt = generator.populate(f, PopulationGenerator.TYPE_INT, popIntC, params.xMin, params.yMin, params.xMax, params.yMax);
                params.populationFloat = generator.populate(f, PopulationGenerator.TYPE_FLOAT, popFloatC, params.xMin, params.yMin, params.xMax, params.yMax);
            }

            drawingThread = new Thread(() -> {
                try (Writer writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(basePath + fileName + ".csv"), "utf-8"))) {
                    // Helper variable to prevent unnecessary re-draws
                    boolean drawn = false;

                    while (!alg.isComplete()) {
                        if (Thread.currentThread().isInterrupted()) {
                            break;
                        }

                        // Progress
                        drawn = false;
                        params.population = alg.progress();

                        // Save to CSV
                        writer.write(alg.csv());

                        // Save screenshot
                        if (screenShots && (alg.getGeneration() % screenShotBreaks == 0 || alg.isComplete() || alg.getGeneration() == 1)) {
                            // Update canvas
                            SwingUtilities.invokeAndWait(() -> {
                                canvas.update(f, params);
                                sidebar.setInfo(alg.info());
                            });

                            // Save screenshot
                            File file = new File(basePath + fileName + "_" + alg.getGeneration() + ".png");
                            canvas.screenShot(file);
                            drawn = true;
                        }

                        // Redraw on each iteration
                        if (continuous && !drawn) {
                            drawn = true;
                            SwingUtilities.invokeAndWait(() -> {
                                canvas.update(f, params);
                                sidebar.setInfo(alg.info());
                            });
                        }
                    }

                    // Draw
                    if (!drawn) {
                        SwingUtilities.invokeAndWait(() -> {
                            canvas.update(f, params);
                            sidebar.setInfo(alg.info());
                        });
                    }
                } catch (Exception e1) {
                    e1.printStackTrace();
                }
            });

            // Start drawing thread
            drawingThread.start();
        });

        sidebar.addStopActionListener((e) -> {
            if (drawingThread != null) {
                try {
                    drawingThread.interrupt();
                    drawingThread.join();
                } catch (InterruptedException e1) {
                    e1.printStackTrace();
                }
            }
        });

        // Add Canvas and sidebar
        panel.add(canvas, BorderLayout.CENTER);
        panel.add(sidebar, BorderLayout.LINE_END);

        // Show Frame
        this.setContentPane(panel);
        this.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        this.pack();
    }
}
