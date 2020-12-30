package com.kru13.httpserver.models.pages;

import android.os.Handler;
import android.text.TextUtils;

import com.kru13.httpserver.models.HttpRequest;
import com.kru13.httpserver.models.HttpResponse;

import java.io.File;
import java.util.Arrays;

public class DirectoryListingsPage extends Page {
    public DirectoryListingsPage(HttpRequest request, File webFile, Handler handler) {
        super(request, webFile, handler);
    }

    @Override
    public HttpResponse getResponse() {
        return new HttpResponse(generateListings(mWebFile));
    }

    private String generateListings(File root) {
        File files[] = root.listFiles();
        StringBuilder body = new StringBuilder();
        body.append("<!DOCTYPE html><html><head><meta charset=\"utf-8\"><title></title></head><body><ul>");

        // Add 2 dots to go up one level
        if (!mRequest.getPath().equals("/")) {
            String requestPathParts[] = mRequest.getPath().split("/");
            body.append("<li><a href=\"").append(TextUtils.join("/", Arrays.copyOf(requestPathParts, requestPathParts.length - 1))).append("/..\">..</a></li>");
        }

        // Build directory listings
        String prefix = mRequest.getPath().equals("/") ? "" : "/";
        if (files != null) {
            for (File file : files) {
                body.append("<li><a href=\"").append(mRequest.getPath()).append(prefix).append(file.getName()).append("\">").append(file.getName()).append("</a></li>");
            }
        }

        body.append("</ul></body></html>\n");
        return body.toString();
    }
}
