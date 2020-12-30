package com.jansimecek.vsb.bia.gui.panels;

import com.jansimecek.vsb.bia.algorithm.Algorithm;
import com.jansimecek.vsb.bia.algorithm.JDEAlgorithm;
import com.jansimecek.vsb.bia.function.IFunction;

public class JDEPanel extends AlgorithmPanel {
    private float tau;

    public JDEPanel() {
        super(10, 1000);
        addInGrid("Tau:", 0.1f, this::setTau);
    }

    @Override
    public Algorithm getAlgorithm(IFunction function) {
        return new JDEAlgorithm(function, getPopSize(), getNumGenerations(), tau);
    }

    private void setTau(float tau) {
        this.tau = tau;
    }
}
