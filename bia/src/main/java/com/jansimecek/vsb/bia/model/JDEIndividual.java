package com.jansimecek.vsb.bia.model;

import org.jzy3d.maths.Coord3d;

public class JDEIndividual {
    public Coord3d p;
    public float F, CR;

    public JDEIndividual() {
        p = new Coord3d();
    }

    public JDEIndividual(float F, float CR) {
        this();
        this.F = F;
        this.CR = CR;
    }

    public JDEIndividual(float x, float y, float z, float F, float CR) {
        p = new Coord3d(x, y, z);
        this.F = F;
        this.CR = CR;
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        } else if (obj == null) {
            return false;
        } else if (!(obj instanceof JDEIndividual)) {
            return false;
        } else {
            JDEIndividual other = (JDEIndividual)obj;
            return other.p.equals(this.p) && this.F == other.F && this.CR == other.CR;
        }
    }
}
