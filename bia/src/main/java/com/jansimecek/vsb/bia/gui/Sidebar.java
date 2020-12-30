package com.jansimecek.vsb.bia.gui;

import com.jansimecek.vsb.bia.function.*;
import com.jansimecek.vsb.bia.gui.panels.*;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionListener;
import java.util.ArrayList;
import java.util.List;

public class Sidebar extends JPanel {
    private SidebarActionListener listener;
    private final String functionLabels[] = {"Sphere", "Rastrigin", "Ackley", "Rosenbrock", "Beale", "Booth", "BukinN6", "CrossInTray", "Pareto"};
    private final String algorithmLabels[] = {"BlindSearchPanel", "Stochastic Hill Climbing", "Simulated Annealing", "Differential Evolution", "JDEAlgorithm", "SOMA", "Evolution Strategy", "Particle Swarm"};
    private List<IFunction> functionList;

    // Components
    private JComboBox<String> cbFunctions, cbAlgorithms;
    private JButton btnDraw, btnStop;
    private JTextArea taInfo;
    private JTextField tfPopInt, tfPopFloat, tfScreenBreaks;
    private JCheckBox cbDrawPopulation, cbContinuous, cbScreenShot;
    private JPanel container;
    private AlgorithmPanel algorithmPanel;
    private GridBagConstraints gbc;

    public Sidebar() {
        // Init functions
        functionList = new ArrayList<>();
        functionList.add(new Sphere());
        functionList.add(new Rastrigin());
        functionList.add(new Ackley());
        functionList.add(new Rosenbrock());
        functionList.add(new Beale());
        functionList.add(new Booth());
        functionList.add(new BukinN6());
        functionList.add(new CrossInTray());
        functionList.add(new Pareto());

        // Init layout
        init();
    }

    public void init() {
        this.setAlignmentY(JPanel.TOP_ALIGNMENT);
        this.setBorder(BorderFactory.createEmptyBorder(0, 0, 0, 0));
        this.setLayout(new BorderLayout());
        this.setPreferredSize(new Dimension(300, 0));

        // Layout container
        container = new JPanel(new GridBagLayout());
        gbc = new GridBagConstraints();
        gbc.anchor = GridBagConstraints.LINE_START;
        gbc.fill = GridBagConstraints.HORIZONTAL;
        gbc.weightx = 1;
        gbc.gridx = 0;
        gbc.gridy = 0;

        // Top and bottom panels
        JPanel topPanel = new JPanel(new GridBagLayout());
        topPanel.setBorder(BorderFactory.createEmptyBorder(10, 10, 0, 10));
        JPanel bottomPanel = new JPanel(new GridBagLayout());
        bottomPanel.setBorder(BorderFactory.createEmptyBorder(0, 10, 10, 10));

        // Init top panel components
        cbFunctions = new JComboBox<>();
        cbFunctions.setModel(new DefaultComboBoxModel<>(functionLabels));
        cbAlgorithms = new JComboBox<>();
        cbAlgorithms.setModel(new DefaultComboBoxModel<>(algorithmLabels));
        btnDraw = new JButton("Draw");
        btnStop = new JButton("Stop");
        tfPopFloat = new JTextField();
        tfPopInt = new JTextField();
        tfScreenBreaks = new JTextField("10");
        cbDrawPopulation = new JCheckBox("Draw population");
        cbContinuous = new JCheckBox("Continuous drawing");
        cbScreenShot = new JCheckBox("Save screenshots");

        // Add to top panel
        gbc.gridy++;
        topPanel.add(new JLabel("  Function:"), gbc);
        gbc.gridy++;
        topPanel.add(cbFunctions, gbc);
        gbc.gridy++;
        topPanel.add(new JLabel("  Algorithm:"), gbc);
        gbc.gridy++;
        topPanel.add(cbAlgorithms, gbc);
        gbc.gridy++;
        topPanel.add(new JSeparator(), gbc);

        // Bottom Panel
        gbc.gridy++;
        bottomPanel.add(cbContinuous, gbc);
        gbc.gridy++;
        bottomPanel.add(new JSeparator(), gbc);
        gbc.gridy++;
        bottomPanel.add(new JLabel("  Float population:"), gbc);
        gbc.gridy++;
        bottomPanel.add(tfPopFloat, gbc);
        gbc.gridy++;
        bottomPanel.add(new JLabel("  Int population:"), gbc);
        gbc.gridy++;
        bottomPanel.add(tfPopInt, gbc);
        gbc.gridy++;
        bottomPanel.add(cbDrawPopulation, gbc);
        gbc.gridy++;
        bottomPanel.add(new JSeparator(), gbc);
        gbc.gridy++;
        bottomPanel.add(new JLabel("  Screenshot breaks:"), gbc);
        gbc.gridy++;
        bottomPanel.add(tfScreenBreaks, gbc);
        gbc.gridy++;
        bottomPanel.add(cbScreenShot, gbc);
        gbc.gridy++;
        bottomPanel.add(new JSeparator(), gbc);
        gbc.gridy++;
        bottomPanel.add(btnDraw, gbc);
        gbc.gridy++;
        bottomPanel.add(btnStop, gbc);
        gbc.gridy++;

        // Add panels to container
        gbc.gridy = 0;
        container.add(topPanel, gbc);
        gbc.gridy = 1;
        algorithmPanel = new BlindSearchPanel();
        container.add(algorithmPanel, gbc);
        gbc.gridy = 2;
        container.add(bottomPanel, gbc);

        // Create info panel
        JPanel infoPanel = new JPanel(new GridBagLayout());
        taInfo = new JTextArea("");
        taInfo.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
        taInfo.setBackground(new Color(210, 210, 210));
//        taInfo.setForeground(Color.WHITE);
        taInfo.setEditable(false);
        infoPanel.add(taInfo, gbc);

        // Add to root panel
        this.add(container, BorderLayout.PAGE_START);
        this.add(infoPanel, BorderLayout.PAGE_END);

        // Add listeners
        btnDraw.addActionListener(e -> {
            listener.onActionClick(
                    e,
                    functionList.get(cbFunctions.getSelectedIndex()),
                    algorithmPanel.getAlgorithm(functionList.get(cbFunctions.getSelectedIndex())),
                    cbContinuous.isSelected(),
                    cbDrawPopulation.isSelected(),
                    tfPopInt.getText().length() > 0 ? Integer.parseInt(tfPopInt.getText()) : 0,
                    tfPopFloat.getText().length() > 0 ? Integer.parseInt(tfPopFloat.getText()) : 0,
                    algorithmPanel.getNumGenerations(),
                    cbScreenShot.isSelected(),
                    tfScreenBreaks.getText().length() > 0 ? Integer.parseInt(tfScreenBreaks.getText()) : 0
            );
        });

        cbAlgorithms.addActionListener(e ->{
            if (algorithmPanel != null) {
                container.remove(algorithmPanel);
            }

            // Set grid and get selected algorithm
            gbc.gridy = 1;
            switch (cbAlgorithms.getSelectedIndex()) {
                case 0:
                    algorithmPanel = new BlindSearchPanel();
                    container.add(algorithmPanel, gbc);
                    break;

                case 1:
                    algorithmPanel = new StochasticHillClimbingPanel();
                    container.add(algorithmPanel, gbc);
                    break;

                case 2:
                    algorithmPanel = new SimulatedAnnealingPanel();
                    container.add(algorithmPanel, gbc);
                    break;

                case 3:
                    algorithmPanel = new DifferentialEvolutionPanel();
                    container.add(algorithmPanel, gbc);
                    break;

                case 4:
                    algorithmPanel = new JDEPanel();
                    container.add(algorithmPanel, gbc);
                    break;

                case 5:
                    algorithmPanel = new SOMAPanel();
                    container.add(algorithmPanel, gbc);
                    break;

                case 6:
                    algorithmPanel = new EvolutionStrategyPanel();
                    container.add(algorithmPanel, gbc);
                    break;

                case 7:
                    algorithmPanel = new ParticleSwarmPanel();
                    container.add(algorithmPanel, gbc);
                    break;

                default:
                    break;
            }

            revalidate();
        });
    }

    public void addSidebarActionListener(SidebarActionListener listener) {
        this.listener = listener;
    }

    public void addStopActionListener(ActionListener listener) { btnStop.addActionListener(listener); }

    public void setInfo(String info) { taInfo.setText(info); }
}
