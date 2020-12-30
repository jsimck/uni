package com.kru13.httpserver.listeners;

import com.kru13.httpserver.interfaces.CameraListener;
import com.kru13.httpserver.models.Payload;

public class CameraCaptureListener implements CameraListener {
    private Payload mPayload;

    public CameraCaptureListener(Payload payload) {
        mPayload = payload;
    }

    @Override
    public void onImageCapture(byte[] image) {
        synchronized (mPayload) {
            mPayload.setData(image);
            mPayload.notifyAll();
        }
    }
}
