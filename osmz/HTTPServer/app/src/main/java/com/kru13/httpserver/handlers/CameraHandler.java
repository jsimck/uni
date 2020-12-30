package com.kru13.httpserver.handlers;

import android.hardware.Camera;
import android.os.Environment;
import android.util.Log;

import com.kru13.httpserver.interfaces.CameraListener;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class CameraHandler {
    private static final String TAG = "CameraHandler";

    private String mFileName;
    private Camera mCamera;
    private List<CameraListener> mListeners;

    private Camera.PictureCallback mCachePictureCallback = new Camera.PictureCallback() {

        @Override
        public void onPictureTaken(byte[] data, Camera camera) {
            for (CameraListener listener : mListeners) {
                listener.onImageCapture(data);
            }
        }
    };

    private Camera.PictureCallback mPictureCallback = new Camera.PictureCallback() {

        @Override
        public void onPictureTaken(byte[] data, Camera camera) {
            try {
                File pictureFile = getOutputMediaFile();
                if (pictureFile == null){
                    Log.d(TAG, "Error creating media file, check storage permissions");
                    return;
                }

                FileOutputStream fos = new FileOutputStream(pictureFile);
                fos.write(data);
                fos.close();

                // Notify camera page of written html
                for (CameraListener listener : mListeners) {
                    listener.onImageCapture(null);
                }
            } catch (FileNotFoundException e) {
                Log.d(TAG, "File not found: " + e.getMessage());
                e.printStackTrace();
            } catch (IOException e) {
                Log.d(TAG, "Error accessing file: " + e.getMessage());
                e.printStackTrace();
            }
        }
    };

    public CameraHandler(String fileName) {
        mFileName = fileName;
        mListeners = new ArrayList<>();
    }

    public void takePicture() {
        try {
            mCamera.takePicture(null, null, mCachePictureCallback);
        } catch (Exception e) {
            Log.d(TAG, "takePicture: " + e.getMessage());
            e.printStackTrace();
        }
    }

    public void takePictureSDCard() {
        try {
            mCamera.takePicture(null, null, mPictureCallback);
        } catch (Exception e) {
            Log.d(TAG, "takePictureSDCard: " + e.getMessage());
            e.printStackTrace();
        }
    }

    private File getOutputMediaFile() throws IOException {
        // Create media folder
        File cameraFolder = getCameraFolder();

        // Create media file
        File mediaFile = new File(cameraFolder, mFileName);
        if (!mediaFile.exists()) {
            mediaFile.createNewFile();
        }

        return mediaFile;
    }

    public void open() {
        try {
            mCamera = Camera.open();
        } catch (Exception e){
            Log.d(TAG, "open: " + e.getMessage());
            e.printStackTrace();
        }
    }

    public void release() {
        if (mCamera != null) {
            mCamera.release();
            mCamera = null;
        }
    }

    public static File getCameraFolder() {
        File mediaFile = new File(Environment.getExternalStorageDirectory(), HttpHandler.WEB_ROOT + File.separator + "camera-data");
        if (!mediaFile.exists()) {
            mediaFile.mkdirs();
        }

        return mediaFile;
    }

    public void registerListener(CameraListener listener) {
        mListeners.add(listener);
    }

    public void removeListener(CameraListener listener) {
        mListeners.remove(listener);
    }

    public Camera getCameraInstance() {
        return mCamera;
    }
}
