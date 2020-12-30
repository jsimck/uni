package com.jansimecek.vsb.bia.model;

public class SPoint {
    public static final String KEYS[] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};
    private String Id;

    public SPoint(String Id) {
        this.Id = Id;
    }

    public static SPoint generate(int index) {
        return new SPoint(SPoint.KEYS[index]);
    }

    public String getId() {
        return Id;
    }

    public void setId(String id) {
        this.Id = id;
    }

    @Override
    public String toString() {
        return Id;
    }
}
