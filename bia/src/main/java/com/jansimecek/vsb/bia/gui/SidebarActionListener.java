package com.jansimecek.vsb.bia.gui;

import com.jansimecek.vsb.bia.algorithm.Algorithm;
import com.jansimecek.vsb.bia.function.IFunction;

import java.awt.event.ActionEvent;

public interface SidebarActionListener {
    void onActionClick(ActionEvent e, IFunction f, Algorithm alg, boolean continuous, boolean popEnabled, int popIntC, int popFloatC, int generations, boolean screenshots, int screenShotBreaks);
}
