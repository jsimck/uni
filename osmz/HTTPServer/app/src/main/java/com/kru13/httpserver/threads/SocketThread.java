package com.kru13.httpserver.threads;

import android.os.Handler;
import android.os.Message;
import android.util.Log;

import com.kru13.httpserver.handlers.HttpHandler;
import com.kru13.httpserver.models.HttpRequest;

import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.util.concurrent.Semaphore;

public class SocketThread implements Runnable {
    private static final String TAG = "SocketThread";

    private Socket mSocket;
    private Handler mHandler;
    private Semaphore mSemaphore;

    public SocketThread(Socket socket, Handler handler, Semaphore semaphore) {
        mSocket = socket;
        mHandler = handler;
        mSemaphore = semaphore;
    }

    @Override
    public void run() {
        try {
            mSemaphore.acquire();

            OutputStream o = mSocket.getOutputStream();
            InputStream i = mSocket.getInputStream();

            // Parse header
            HttpRequest request = new HttpRequest(i);
            HttpHandler response = new HttpHandler(request, o, mHandler);
            response.resolve();

            // Log header mRequest
            Message m = mHandler.obtainMessage();
            m.obj = "Spawn thread id: " + Long.toString(Thread.currentThread().getId()) + "\n" + request.toString() + "\n";
            m.sendToTarget();

            // Close mSocket
            mSocket.close();
            Log.d("SERVER", "Socket Closed");
        } catch (Exception e) {
            Log.d(TAG, "run: " + e.getMessage());
            e.printStackTrace();
        } finally {
            mSemaphore.release();
        }
    }
}
