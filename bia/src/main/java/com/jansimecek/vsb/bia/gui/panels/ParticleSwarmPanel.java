package com.jansimecek.vsb.bia.gui.panels;

import com.jansimecek.vsb.bia.algorithm.Algorithm;
import com.jansimecek.vsb.bia.algorithm.EvolutionStrategy;
import com.jansimecek.vsb.bia.algorithm.ParticleSwarmAlgorithm;
import com.jansimecek.vsb.bia.function.IFunction;

public class ParticleSwarmPanel extends AlgorithmPanel {
    private float c1, c2, vMax;

    public ParticleSwarmPanel() {
        super(100, 1000);
        addInGrid("vMax:", 1.0f, this::setvMax);
        addInGrid("c1:", 0.1f, this::setC1);
        addInGrid("c2:", 0.1f, this::setC2);

    }

    @Override
    public Algorithm getAlgorithm(IFunction function) {
        return new ParticleSwarmAlgorithm(function, getPopSize(), getNumGenerations(), vMax, c1, c2);
    }

    private void setC1(float c1) { this.c1 = c1; }
    private void setC2(float c2) { this.c2 = c2; }
    private void setvMax(float vMax) { this.vMax = vMax; }
}
