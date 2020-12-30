package com.kru13.httpserver.models.pages;

import android.os.Handler;

import com.kru13.httpserver.models.HttpRequest;
import com.kru13.httpserver.models.HttpResponse;

import java.io.File;
import java.io.IOException;

public class ErrorPage extends Page {
    public ErrorPage(HttpRequest request, File webFile, Handler handler) {
        super(request, webFile, handler);
    }

    @Override
    public HttpResponse getResponse() {
        String body = "<!DOCTYPE html> <html> <head> <meta charset=\"utf-8\"> <title>404 Not Found</title> <style media=\"screen\"> body { text-align: center; padding: 100px; background: #fafafa; font-family: sans-serif; font-weight: 300; } h1 { font-size: 120px; margin-bottom: 20px; color: #222222; } h2 { color: #686868; } </style> </head> <body> <h1>404</h1> <h2>Not Found</h2> </body> </html>";
        return new HttpResponse(404, "Not Found", HttpResponse.CONTENT_TYPE_HTML, body.length(), body);
    }
}
