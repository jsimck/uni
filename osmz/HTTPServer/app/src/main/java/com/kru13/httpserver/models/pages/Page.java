package com.kru13.httpserver.models.pages;

import android.os.Handler;
import android.os.Message;

import com.kru13.httpserver.models.HttpRequest;
import com.kru13.httpserver.models.HttpResponse;
import com.kru13.httpserver.models.Payload;

import java.io.File;
import java.io.IOException;

public abstract class Page {
    protected HttpRequest mRequest;
    protected File mWebFile;
    protected Handler mHandler;

    public Page(HttpRequest request, File webFile, Handler handler) {
        mRequest = request;
        mWebFile = webFile;
        mHandler = handler;
    }

    public void sendMessage(Message message) {
        Message m = mHandler.obtainMessage();
        m.obj = message;
        m.sendToTarget();
    }

    public void sendMessage(Payload payload) {
        Message m = mHandler.obtainMessage();
        m.obj = payload;
        m.sendToTarget();
    }

    public abstract HttpResponse getResponse();
}
