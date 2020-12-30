package com.jansimecek.vsb.bia.gui.panels;

import com.jansimecek.vsb.bia.algorithm.Algorithm;
import com.jansimecek.vsb.bia.algorithm.SOMAAlgorithm;
import com.jansimecek.vsb.bia.function.IFunction;

public class SOMAPanel extends AlgorithmPanel {
    private float pathLength, step, PRT;

    public SOMAPanel() {
        super(10, 10);
        addInGrid("pathLength", 3.0f, this::setPathLength);
        addInGrid("step", 0.11f, this::setStep);
        addInGrid("PRT", 0.1f, this::setPRT);
    }

    @Override
    public Algorithm getAlgorithm(IFunction function) {
        return new SOMAAlgorithm(function, getPopSize(), getNumGenerations(), pathLength, step, PRT);
    }

    private void setPathLength(float pathLength) { this.pathLength = pathLength; }
    private void setStep(float step) { this.step = step; }
    private void setPRT(float PRT) { this.PRT = PRT; }
}
