package com.jansimecek.vsb.bia.gui;

import com.jansimecek.vsb.bia.algorithm.TSPGeneticAlgorithm;
import com.jansimecek.vsb.bia.model.City;
import com.jansimecek.vsb.bia.model.TSPIndividual;

import javax.swing.*;
import java.awt.*;
import java.awt.Canvas;
import java.awt.geom.Line2D;
import java.util.ArrayList;
import java.util.List;

public class TSPWindow extends JFrame {
    private final int width = 1000, height = 700;
    private List<City> cities;
    private Canvas canvas;
    private JPanel sidebar;
    private JButton start;
    private JTextArea info;
    private JTextField tfPopSize, tfGenerations;

    public TSPWindow() {
        super("BIA");

        // Create cities
        cities = new ArrayList<>();
        cities.add(new City("A",60, 200));
        cities.add(new City("B",180, 200));
        cities.add(new City("C",80, 180));
        cities.add(new City("D",140, 180));
        cities.add(new City("E",20, 160));
        cities.add(new City("F",100, 160));
        cities.add(new City("G",200, 160));
        cities.add(new City("H",140, 140));
        cities.add(new City("I",40, 120));
        cities.add(new City("J",100, 120));
        cities.add(new City("K",180, 100));
        cities.add(new City("L",60, 80));
        cities.add(new City("M",120, 80));
        cities.add(new City("N",180, 60));
        cities.add(new City("O",20, 40));
        cities.add(new City("P",100, 40));
        cities.add(new City("Q",200, 40));
        cities.add(new City("R",20, 20));
        cities.add(new City("S",60, 20));
        cities.add(new City("T",160, 20));

        // Init gui
        init();
    }

    private void init() {
        // Init canvas
        canvas = new Canvas();
        canvas.setPreferredSize(new Dimension(width, height));
        canvas.setBackground(Color.WHITE);

        // Init sidebar
        initSidebar();

        // Add components to panel
        JPanel panel = new JPanel(new BorderLayout());
        panel.add(canvas, BorderLayout.CENTER);
        panel.add(sidebar, BorderLayout.LINE_END);

        this.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        this.setContentPane(panel);
        this.pack();
    }

    private void initSidebar() {
        // Init sidebar
        sidebar = new JPanel(new BorderLayout());
        sidebar.setAlignmentY(JPanel.TOP_ALIGNMENT);
        sidebar.setBorder(BorderFactory.createEmptyBorder(0, 0, 0, 0));
        sidebar.setLayout(new BorderLayout());
        sidebar.setPreferredSize(new Dimension(300, 0));

        // Layout topPanel
        JPanel topPanel = new JPanel(new GridBagLayout());
        topPanel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.anchor = GridBagConstraints.LINE_START;
        gbc.fill = GridBagConstraints.HORIZONTAL;
        gbc.weightx = 1;
        gbc.gridx = 0;
        gbc.gridy = 0;

        // Init components
        start = new JButton("Start");
        tfPopSize = new JTextField("50");
        tfGenerations = new JTextField("1000");

        // Add components to sidebar
        gbc.gridy++;
        topPanel.add(new JLabel("  Population size:"), gbc);
        gbc.gridy++;
        topPanel.add(tfPopSize, gbc);
        gbc.gridy++;
        topPanel.add(new JLabel("  Number of generations:"), gbc);
        gbc.gridy++;
        topPanel.add(tfGenerations, gbc);
        gbc.gridy++;
        topPanel.add(new JSeparator(), gbc);
        gbc.gridy++;
        topPanel.add(start, gbc);
        gbc.gridy++;

        // Init info panel
        JPanel infoPanel = new JPanel(new GridBagLayout());
        info = new JTextArea("");
        info.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
        info.setBackground(new Color(210, 210, 210));
        info.setEditable(false);
        infoPanel.add(info, gbc);

        // Top and bottom panels
        sidebar.add(topPanel, BorderLayout.PAGE_START);
        sidebar.add(infoPanel, BorderLayout.PAGE_END);

        // Add action listeners
        start.addActionListener((e) -> {
            TSPGeneticAlgorithm tspGeneticAlgorithm = new TSPGeneticAlgorithm(
                    (tfPopSize.getText().length() > 0) ? Integer.parseInt(tfPopSize.getText()) : 1000,
                    (tfGenerations.getText().length() > 0) ? Integer.parseInt(tfGenerations.getText()) : 50,
                    cities
            );

            new Thread(() -> {
                float nFitness, oFitness = 0;
                while (!tspGeneticAlgorithm.isComplete()) {
                    try {
                        TSPIndividual ind = tspGeneticAlgorithm.progress();
                        nFitness = ind.fitness();

                        SwingUtilities.invokeAndWait(() -> {
                            try {
                                drawTSP((Graphics2D) canvas.getGraphics(), width, height, ind);
                                info.setText(tspGeneticAlgorithm.info());
                            } catch (Exception exp) {
                                exp.printStackTrace();
                            }
                        });

                        if (nFitness != oFitness) {
                            oFitness = nFitness;
                            Thread.sleep(100);
                        }
                    } catch (Exception exp) {
                        exp.printStackTrace();
                    }
                }
            }).start();
        });
    }

    private void drawTSP(Graphics2D g, int width, int height, TSPIndividual ind) {
        final int N = ind.cities.size();
        int pointSize = 20;
        int offset = pointSize / 2;

        // Define scale to span graph across the whole canvas
        float scaleW = width / 215.0f;
        float scaleH = height / 215.0f;

        // Clear canvas
        g.clearRect(0, 0, width, height);

        // Draw lines
        g.setColor(Color.RED);
        g.setStroke(new BasicStroke(2));
        for (int i = 0; i < N - 1; i++) {
            g.drawLine(
                (int) (ind.cities.get(i).x * scaleW),
                (int) (ind.cities.get(i).y * scaleH),
                (int) (ind.cities.get(i + 1).x * scaleW),
                (int) (ind.cities.get(i + 1).y * scaleH)
            );
        }

        // Draw last line
        g.drawLine(
            (int) (ind.cities.get(0).x * scaleW),
            (int) (ind.cities.get(0).y * scaleH),
            (int) (ind.cities.get(N - 1).x * scaleW),
            (int) (ind.cities.get(N - 1).y * scaleH)
        );

        // Draw Points
        g.setColor(Color.BLUE);
        for (int i = 0; i < N; i++) {
            g.fillOval(
                (int) (ind.cities.get(i).x * scaleW) - offset,
                (int) (ind.cities.get(i).y * scaleH) - offset,
                pointSize,
                pointSize
            );
        }
    }
}
