package com.kru13.httpserver.models;

public class Payload {
    private int mId;
    private byte[] mData;

    public Payload(int id, byte[] data) {
        mId = id;
        mData = data;
    }

    public int getId() { return mId; }
    public byte[] getData() { return mData; }

    public void setId(int mId) { this.mId = mId; }
    public void setData(byte[] mData) { this.mData = mData; }
}
