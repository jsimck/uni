package com.jansimecek.vsb.bia.gui.panels;

import com.jansimecek.vsb.bia.algorithm.Algorithm;
import com.jansimecek.vsb.bia.algorithm.DifferentialEvolution;
import com.jansimecek.vsb.bia.function.IFunction;

public class DifferentialEvolutionPanel extends AlgorithmPanel {
    private float F, CR;

    public DifferentialEvolutionPanel() {
        super(10, 10);
        addInGrid("F:", 0.5f, this::setF);
        addInGrid("CR:", 0.9f, this::setCR);
    }

    @Override
    public Algorithm getAlgorithm(IFunction function) {
        return new DifferentialEvolution(function, getPopSize(), getNumGenerations(), F, CR);
    }

    private void setF(float f) {
        F = f;
    }

    private void setCR(float CR) {
        this.CR = CR;
    }
}
