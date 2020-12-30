package com.jansimecek.vsb.bia.model;

public class City {
    public String name;
    public float x, y;

    public City(String name, int x, int y) {
        this.name = name;
        this.x = x;
        this.y = y;
    }

    @Override
    public boolean equals(Object obj) {
        if (obj == null) {
            return false;
        } if (!(obj instanceof City)) {
            return false;
        } else {
            City other = (City) obj;
            return x == other.x && y == other.y && name.equals(other.name);
        }
    }
}
