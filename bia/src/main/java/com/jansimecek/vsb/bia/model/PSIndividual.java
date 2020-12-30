package com.jansimecek.vsb.bia.model;

import org.jzy3d.maths.Coord3d;

public class PSIndividual {
    public Coord3d p, pBest;
    public float v0, v1;

    public PSIndividual(Coord3d point, float v0, float v1) {
        this.p = point;
        this.pBest = point.clone();
        this.v0 = v0;
        this.v1 = v1;
    }

    public PSIndividual(Coord3d pBest) {
        this.p = new Coord3d();
        this.pBest = pBest.clone();
        this.v0 = 0;
        this.v1 = 0;
    }

    public PSIndividual(PSIndividual individual) {
        this.p = individual.p.clone();
        this.pBest = individual.pBest.clone();
        this.v0 = individual.v0;
        this.v1 = individual.v1;
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        } else if (obj == null) {
            return false;
        } else if (!(obj instanceof PSIndividual)) {
            return false;
        } else {
            PSIndividual other = (PSIndividual) obj;
            return other.p.equals(this.p) && other.pBest.equals(this.pBest);
        }
    }
}
