package com.kru13.httpserver.services;

import android.app.IntentService;
import android.app.Service;
import android.content.Intent;
import android.graphics.Color;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.support.annotation.IntDef;
import android.support.annotation.Nullable;
import android.util.Log;
import android.widget.Button;
import android.widget.TextView;

import com.kru13.httpserver.handlers.CameraHandler;
import com.kru13.httpserver.listeners.CameraCaptureListener;
import com.kru13.httpserver.models.Payload;
import com.kru13.httpserver.threads.SocketServer;

public class HttpBackgroundService extends Service {
    private static final String TAG = "HttpBackgroundService";

    public static final int PAYLOAD_CAMERA_CACHED = 0;
    public static final int PAYLOAD_CAMERA_SDCARD = 1;

    private SocketServer mSocketServer;
    private CameraHandler mCameraHandler;

    @Override
    public void onCreate() {
        super.onCreate();
    }

    @Override
    public void onDestroy() {
        super.onDestroy();

        // Release camera
        mCameraHandler.release();

        // Close socket server
        mSocketServer.close();
        try {
            mSocketServer.join();
        } catch (InterruptedException e) {
            Log.d(TAG, "onDestroy: " + e.getMessage());
            e.printStackTrace();
        }

        Log.d(TAG, "onDestroy: SERVICE STOPPED");
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.d(TAG, "onStartCommand: SERVICE STARTED");

        // Open camera
        mCameraHandler = new CameraHandler("photo.jpg");
        mCameraHandler.open();

        // Thread handler
        Handler handler = new Handler() {
            @Override
            public void handleMessage(Message msg) {
                super.handleMessage(msg);

                if (msg.obj instanceof Payload) {
                    // Reopen camera instance
                    if (mCameraHandler.getCameraInstance() == null) {
                        mCameraHandler.open();
                    }

                    if (((Payload) msg.obj).getId() == PAYLOAD_CAMERA_CACHED) {
                        mCameraHandler.registerListener(new CameraCaptureListener((Payload) msg.obj));
                        mCameraHandler.takePicture();
                    } else if (((Payload) msg.obj).getId() == PAYLOAD_CAMERA_SDCARD) {
                        mCameraHandler.registerListener(new CameraCaptureListener((Payload) msg.obj));
                        mCameraHandler.takePictureSDCard();
                    }
                }
            }
        };

        // Start server
        mSocketServer = new SocketServer(handler);
        mSocketServer.start();

        return Service.START_STICKY;
    }

    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }
}
