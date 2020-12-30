package com.jansimecek.vsb.bia.gui.panels;

import com.jansimecek.vsb.bia.algorithm.Algorithm;
import com.jansimecek.vsb.bia.algorithm.EvolutionStrategy;
import com.jansimecek.vsb.bia.function.IFunction;

import javax.swing.*;

public class EvolutionStrategyPanel extends AlgorithmPanel {
    private float deviation, mutationIntensity;
    private boolean isParentAndDescendant;

    public EvolutionStrategyPanel() {
        super(100, 1000);
        addInGrid("Deviation:", 1.5f, this::setDeviation);
        addInGrid("Mutation intensity:", 0.98f, this::setMutationIntensity);
        addInGrid("Use E(a + b) strategy:", false, this::setParentAndDescendant);
    }

    @Override
    public Algorithm getAlgorithm(IFunction function) {
        return new EvolutionStrategy(function, getPopSize(), getNumGenerations(), deviation, mutationIntensity, isParentAndDescendant);
    }

    private void setDeviation(float deviation) { this.deviation = deviation; }
    private void setParentAndDescendant(boolean parentAndDescendant) { isParentAndDescendant = parentAndDescendant; }
    private void setMutationIntensity(float mutationIntensity) { this.mutationIntensity = mutationIntensity; }
}
