package com.kru13.httpserver.models.pages;

import android.os.Handler;
import android.util.Log;
import android.webkit.MimeTypeMap;

import com.kru13.httpserver.models.HttpRequest;
import com.kru13.httpserver.models.HttpResponse;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileReader;
import java.io.IOException;
import java.nio.ByteBuffer;

public class StoragePage extends Page {
    private static final String TAG = "StoragePage";

    public StoragePage(HttpRequest request, File webFile, Handler handler) {
        super(request, webFile, handler);
    }

    @Override
    public HttpResponse getResponse() {
        HttpResponse response = new HttpResponse(200, "OK");
        String mime = getMimeType(mRequest.getPath());
        response.setContentType(mime);

        try {
            if (mime.startsWith("text/")) {
                String body = readString(mWebFile);
                response.setBody(body);
                response.setContentLength(body.length());
            } else {
                byte[] body = readBinary(mWebFile);
                response.setBody(ByteBuffer.wrap(body));
                response.setContentLength((int) mWebFile.length());
            }
        } catch (IOException e) {
            Log.d(TAG, "getResponse: " + e.getMessage());
            e.printStackTrace();
        }

        return response;
    }

    public static byte[] readBinary(File file) throws IOException {
        byte buffer[] = new byte[(int) file.length()];
        FileInputStream reader = new FileInputStream(file);

        reader.read(buffer);
        reader.close();

        return buffer;
    }

    public static String readString(File file) throws IOException {
        StringBuilder buffer = new StringBuilder();
        BufferedReader reader = new BufferedReader(new FileReader(file));

        String line;
        while ((line = reader.readLine()) != null) {
            buffer.append(line);
        }
        reader.close();

        return buffer.toString();
    }

    public static String getMimeType(String path) {
        String pathParts[] = path.split("\\.");
        String fileExtension = pathParts[pathParts.length - 1];

        // Missing from the map
        if (fileExtension.equals("js")) {
            return "text/javascript";
        }

        return MimeTypeMap.getSingleton().getMimeTypeFromExtension(fileExtension);
    }
}
