package com.jansimecek.vsb.bia.gui.panels;

import com.jansimecek.vsb.bia.algorithm.Algorithm;
import com.jansimecek.vsb.bia.function.*;

import javax.swing.*;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import java.awt.*;
import java.util.function.Consumer;

public abstract class AlgorithmPanel extends JPanel {
    // Helpers
    protected GridBagConstraints gbc;

    // Alg base values
    private int popSize;
    private int numGenerations;

    public AlgorithmPanel(int popSize, int numGenerations) {
        this.popSize = popSize;
        this.numGenerations = numGenerations;

        // Init gbc
        gbc = new GridBagConstraints();
        gbc.anchor = GridBagConstraints.LINE_START;
        gbc.fill = GridBagConstraints.HORIZONTAL;
        gbc.weightx = 1;
        gbc.ipady = 2;
        gbc.gridx = 0;
        gbc.gridy = 0;

        // Init layout
        init();
    }

    private JTextField addInGrid(String label, Number value) {
        // Add Label
        gbc.gridy++;
        add(new JLabel("  " + label), gbc);

        // Add text field
        gbc.gridy++;
        JTextField tf = new JTextField(value.toString());
        add(tf, gbc);

        return tf;
    }

    private JCheckBox addInGrid(String label, boolean value) {
        // Add text field
        gbc.gridy++;
        JCheckBox cb = new JCheckBox(label);
        cb.setSelected(value);
        add(cb, gbc);

        return cb;
    }

    protected void addInGrid(String label, Integer value, Consumer<Integer> setter) {
        // Accept value
        setter.accept(value);
        JTextField tf = addInGrid(label, value);

        // Add on change listener
        tf.getDocument().addDocumentListener(new DocumentListener() {
            @Override
            public void insertUpdate(DocumentEvent e) { changedUpdate(e); }

            @Override
            public void removeUpdate(DocumentEvent e) {
                changedUpdate(e);
            }

            @Override
            public void changedUpdate(DocumentEvent e) {
                try {
                    setter.accept(new Integer(tf.getText()));
                } catch (Exception ignored) { }
            }
        });
    }

    protected void addInGrid(String label, Float value, Consumer<Float> setter) {
        // Accept value
        setter.accept(value);
        JTextField tf = addInGrid(label, value);

        // Add on change listener
        tf.getDocument().addDocumentListener(new DocumentListener() {
            @Override
            public void insertUpdate(DocumentEvent e) {
                changedUpdate(e);
            }

            @Override
            public void removeUpdate(DocumentEvent e) {
                changedUpdate(e);
            }

            @Override
            public void changedUpdate(DocumentEvent e) {
                try {
                    setter.accept(new Float(tf.getText()));
                } catch (Exception ignored) { }
            }
        });
    }

    protected void addInGrid(String label, boolean value, Consumer<Boolean> setter) {
        // Accept value
        setter.accept(value);
        JCheckBox cb = addInGrid(label, value);

        // Add on change listener
        cb.addChangeListener(e -> setter.accept(cb.isSelected()));
    }

    private void init() {
        // Init layout
        setLayout(new GridBagLayout());
        setBorder(BorderFactory.createEmptyBorder(0, 10, 0, 10));

        // Add population
        addInGrid("Population size:", popSize, this::setPopSize);
        addInGrid("Number of numGenerations:", numGenerations, this::setNumGenerations);
    }

    public abstract Algorithm getAlgorithm(IFunction function);

    public int getPopSize() {
        return popSize;
    }

    private void setPopSize(int popSize) {
        this.popSize = popSize;
    }

    public int getNumGenerations() { return numGenerations; }

    private void setNumGenerations(int numGenerations) {
        this.numGenerations = numGenerations;
    }
}
