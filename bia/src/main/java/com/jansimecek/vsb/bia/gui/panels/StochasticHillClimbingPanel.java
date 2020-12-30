package com.jansimecek.vsb.bia.gui.panels;

import com.jansimecek.vsb.bia.algorithm.Algorithm;
import com.jansimecek.vsb.bia.algorithm.StochasticHillClimbing;
import com.jansimecek.vsb.bia.function.IFunction;

public class StochasticHillClimbingPanel extends AlgorithmPanel {
    private float radius;
    private float alpha;

    public StochasticHillClimbingPanel() {
        super(100, 10);
        addInGrid("Radius (0-1)", 0.3f, this::setRadius);
        addInGrid("Alpha (0-1)", 0.9f, this::setAlpha);
    }

    @Override
    public Algorithm getAlgorithm(IFunction function) {
        return new StochasticHillClimbing(function, getPopSize(), getNumGenerations(), radius, alpha);
    }

    private void setRadius(float radius) { this.radius = radius; }
    private void setAlpha(float alpha) {
        this.alpha = alpha;
    }
}
