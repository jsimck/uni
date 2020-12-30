package com.jansimecek.vsb.bia.gui.panels;

import com.jansimecek.vsb.bia.algorithm.BlindAlgorithm;
import com.jansimecek.vsb.bia.algorithm.Algorithm;
import com.jansimecek.vsb.bia.function.IFunction;

public class BlindSearchPanel extends AlgorithmPanel {
    public BlindSearchPanel() {
        super(1000, 100);
    }

    @Override
    public Algorithm getAlgorithm(IFunction function) {
        return new BlindAlgorithm(function, getPopSize(), getNumGenerations());
    }
}
