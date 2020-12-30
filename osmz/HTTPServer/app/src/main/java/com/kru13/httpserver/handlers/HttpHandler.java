package com.kru13.httpserver.handlers;

import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

import com.kru13.httpserver.models.HttpRequest;
import com.kru13.httpserver.models.HttpResponse;
import com.kru13.httpserver.models.Payload;
import com.kru13.httpserver.models.pages.CGIPage;
import com.kru13.httpserver.models.pages.CameraPage;
import com.kru13.httpserver.models.pages.DirectoryListingsPage;
import com.kru13.httpserver.models.pages.ErrorPage;
import com.kru13.httpserver.models.pages.StoragePage;

import java.io.BufferedWriter;
import java.io.DataOutputStream;
import java.io.File;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.nio.ByteBuffer;

public class HttpHandler {
    private static final String TAG = "HttpHandler";
    private static final String BOUNDARY = "--boundary";

    public static String WEB_ROOT = "www";

    private HttpRequest mRequest;
    private OutputStream mStream;
    private Handler mHandler;

    public HttpHandler(HttpRequest request, OutputStream stream, Handler handler) {
        mRequest = request;
        mStream = stream;
        mHandler = handler;
    }

    public void resolve() {
        try {
            // Get web root on sdcard
            File root = new File(Environment.getExternalStorageDirectory(), WEB_ROOT + mRequest.getPath());
            HttpResponse response;

            // Streams
            BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(mStream));
            DataOutputStream binaryWriter = new DataOutputStream(mStream);

            if (mRequest.getPath().startsWith("/cgi-bin/")) {
                response = new CGIPage(mRequest, root, mHandler).getResponse();
            } else if (mRequest.getPath().equals("/photo-cache.jpg")) {
                // Special path used to get cached camera feed
                response = new CameraPage(mRequest, root, mHandler).getCameraFeed();
            } else if (mRequest.getPath().equals("/camera-mjpeg")) {
                // Show camera using motion jpeg
                response = new HttpResponse(200, "OK");
                response.setContentType("multipart/x-mixed-replace;boundary=" + BOUNDARY);
                writer.write(response.headersToString());

                // Boundary + mjpeg
                response = new CameraPage(mRequest, root, mHandler).getCameraFeed();
                writer.write(BOUNDARY);
                writer.write(response.headersToString());
                writer.flush();
                binaryWriter.write(getBinaryData((ByteBuffer) response.getBody()));
                binaryWriter.write("\n".getBytes());
                binaryWriter.flush();

                // Repeat for other frames
                while (true) {
                    response = new CameraPage(mRequest, root, mHandler).getCameraFeed();
                    writer.write(BOUNDARY);
                    writer.write(response.headersToString());
                    writer.flush();
                    binaryWriter.write(getBinaryData((ByteBuffer) response.getBody()));
                    binaryWriter.write("\n".getBytes());
                    binaryWriter.flush();
                }
            } else if (mRequest.getPath().equals("/camera-cached")) {
                // Show camera capturing and loading images from cache
                response = new CameraPage(mRequest, root, mHandler).getCachedResponse();
            } else if (mRequest.getPath().equals("/camera")) {
                // Show camera capturing and loading images from sdcard
                response = new CameraPage(mRequest, root, mHandler).getResponse();
            } else if (root.isFile()){
                // Process file from sdcard
                response = new StoragePage(mRequest, root, mHandler).getResponse();
            } else if (root.listFiles() != null) {
                // Path is not file, but directory containing files
                response = new DirectoryListingsPage(mRequest, root, mHandler).getResponse();
            } else {
                // 404 page, file or directory not found
                response = new ErrorPage(mRequest, root, mHandler).getResponse();
            }

            // Write to output mStream
            if (response.isBinary()) {
                // Write header
                writer.write(response.headersToString());
                writer.flush();

                // Write binary data
                binaryWriter.write(getBinaryData((ByteBuffer) response.getBody()));
                binaryWriter.flush();
            } else {
                // Write header and text data
                writer.write(response.headersToString());
                writer.write((String) response.getBody());
                writer.flush();
            }
        } catch (Exception e) {
            Log.d(TAG, "resolve: " + e.getMessage());
            e.printStackTrace();
        }
    }

    public static byte[] getBinaryData(ByteBuffer buffer) {
        byte[] data = new byte[buffer.remaining()];
        buffer.get(data);

        return data;
    }

    private void sendMessage(String message) {
        Message m = mHandler.obtainMessage();
        m.obj = message;
        m.sendToTarget();
    }

    private void sendMessage(Payload message) {
        Message m = mHandler.obtainMessage();
        m.obj = message;
        m.sendToTarget();
    }
}
