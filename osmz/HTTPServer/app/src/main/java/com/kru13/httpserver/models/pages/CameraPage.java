package com.kru13.httpserver.models.pages;

import android.os.Handler;
import android.util.Log;

import com.kru13.httpserver.models.HttpRequest;
import com.kru13.httpserver.models.HttpResponse;
import com.kru13.httpserver.models.Payload;
import com.kru13.httpserver.services.HttpBackgroundService;

import java.io.File;
import java.nio.ByteBuffer;

public class CameraPage extends Page {
    private static final String TAG = "CameraPage";

    public CameraPage(HttpRequest request, File webFile, Handler handler) {
        super(request, webFile, handler);
    }

    @Override
    public HttpResponse getResponse() {
        Payload p = new Payload(HttpBackgroundService.PAYLOAD_CAMERA_SDCARD, null);
        sendMessage(p);

        // Wait till sdcard get's saved
        synchronized (p) {
            try {
                p.wait();
            } catch (InterruptedException e) {
                Log.d(TAG, "getResponse: " + e.getMessage());
                e.printStackTrace();
            }
        }

        return new HttpResponse("<html><head><title>Camera feed</title><meta http-equiv=\"refresh\" content=\"1;url=http://localhost:12345/camera\"></head><body><h1>Camera Feed SDCard:</h1><img width=\"50%%\" src=\"/camera-data/photo.jpg\"></body></html>");
    }

    public HttpResponse getCachedResponse() {
        return new HttpResponse("<html><head><title>Camera feed</title><meta http-equiv=\"refresh\" content=\"1;url=http://localhost:12345/camera-cached\"></head><body><h1>Camera Feed Cached:</h1><img width=\"50%%\" src=\"/photo-cache.jpg\"></body></html>");
    }

    public HttpResponse getCameraFeed() {
        Payload p = new Payload(HttpBackgroundService.PAYLOAD_CAMERA_CACHED, null);
        sendMessage(p);

        // Wait for picture
        synchronized (p) {
            try {
                p.wait();
            } catch (InterruptedException e) {
                Log.d(TAG, "getCameraFeed: " + e.getMessage());
                e.printStackTrace();
            }
        }

        // Return image if data not null
        if (p.getData() != null) {
            return new HttpResponse(200, "OK", HttpResponse.CONTENT_TYPE_JPEG, p.getData().length, ByteBuffer.wrap(p.getData()));
        } else {
            return new ErrorPage(mRequest, mWebFile, mHandler).getResponse();
        }
    }
}
