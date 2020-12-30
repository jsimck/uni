package com.jansimecek.vsb.bia.gui.panels;

import com.jansimecek.vsb.bia.algorithm.Algorithm;
import com.jansimecek.vsb.bia.algorithm.SimulatedAnnealing;
import com.jansimecek.vsb.bia.function.IFunction;

public class SimulatedAnnealingPanel extends AlgorithmPanel {
    private float finalTemperature;
    private float temperature;
    private float alpha;
    private float radius;

    public SimulatedAnnealingPanel() {
        super(1000, 2000);
        addInGrid("Temperature:", 2000.0f, this::setTemperature);
        addInGrid("Final Temperature:", 0.000001f, this::setFinalTemperature);
        addInGrid("Alpha (0-1):", 0.99f, this::setAlpha);
        addInGrid("Radius (0-1):", 0.5f, this::setRadius);
    }

    @Override
    public Algorithm getAlgorithm(IFunction function) {
        return new SimulatedAnnealing(function, getPopSize(), getNumGenerations(), temperature, finalTemperature, alpha, radius);
    }

    private void setTemperature(float temperature) {
        this.temperature = temperature;
    }
    private void setFinalTemperature(float finalTemperature) { this.finalTemperature = finalTemperature; }
    private void setAlpha(float alpha) {
        this.alpha = alpha;
    }
    private void setRadius(float radius) { this.radius = radius; }
}
