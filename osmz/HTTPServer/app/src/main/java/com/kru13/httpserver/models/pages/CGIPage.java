package com.kru13.httpserver.models.pages;

import android.os.Handler;
import android.util.Log;

import com.kru13.httpserver.models.HttpRequest;
import com.kru13.httpserver.models.HttpResponse;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;

public class CGIPage extends Page {
    private static final String TAG = "CGIPage";

    public CGIPage(HttpRequest request, File webFile, Handler handler) {
        super(request, webFile, handler);
    }

    @Override
    public HttpResponse getResponse() {
        HttpResponse response;
        String command = mRequest.getPath().replace("/cgi-bin/", ""). replaceAll("%20", " ");

        try {
            Runtime r = Runtime.getRuntime();
            Process p = r.exec(command);
            InputStream pInputStream = p.getInputStream();

            // Wait till finished
            p.waitFor();

            // Read stream
            String pResponse = readFully(pInputStream);
            response = new HttpResponse(
                "<html><head><style> code {padding: .2rem .4rem; font-size: 90%; color: #bd4147; background-color: #f7f7f9; border-radius: .25rem; } " +
                "pre {background: #fafafa; padding: 25px; } </style>" +
                "</head><body><h1>Process output: <code>" + command + "</code></h1>" +
                "<pre>" + pResponse + "</pre></body></html>"
            );
        } catch (InterruptedException | IOException e) {
            response = new ErrorPage(mRequest, mWebFile, mHandler).getResponse();
            Log.d(TAG, "getResponse: " + e.getMessage());
            e.printStackTrace();
        }

        return response;
    }

    private String readFully(InputStream is) throws IOException {
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        byte[] buffer = new byte[1024];
        int length = 0;

        while ((length = is.read(buffer)) != -1) {
            baos.write(buffer, 0, length);
        }

        return baos.toString("UTF-8");
    }
}
