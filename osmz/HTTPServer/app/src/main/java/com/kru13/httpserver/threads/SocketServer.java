package com.kru13.httpserver.threads;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.concurrent.Semaphore;

import android.os.Handler;
import android.util.Log;

public class SocketServer extends Thread {
    private static final String TAG = "SocketServer";

    ServerSocket mServerSocket;
    private final Semaphore mSemaphore;
    private Handler mHandler;
    private boolean mRunning;

    public final int port = 12345;
    public final int maxThreads = 20;

    public SocketServer(Handler handler) {
        mHandler = handler;
        mSemaphore = new Semaphore(maxThreads);
    }

    public void close() {
        try {
            mServerSocket.close();
        } catch (IOException e) {
            Log.d(TAG, "close: " + e.getMessage());
            e.printStackTrace();
        }
        mRunning = false;
    }

    public void run() {
        try {
            Log.d("SERVER", "Creating Socket");
            mServerSocket = new ServerSocket(port);
            mRunning = true;

            while (mRunning) {
                Log.d(TAG, "Socket Waiting for connection");
                Socket s = mServerSocket.accept();
                Log.d(TAG, "Socket Accepted");

                Thread t = new Thread(new SocketThread(s, mHandler, mSemaphore));
                t.start();
            }
        } catch (IOException e) {
            if (mServerSocket != null && mServerSocket.isClosed()) {
                Log.d(TAG, "Normal exit: " + e.getMessage());
                e.printStackTrace();
            } else {
                Log.d(TAG, "Error: " + e.getMessage());
                e.printStackTrace();
            }
        } catch (Exception e) {
            Log.d(TAG, "run: " + e.getMessage());
            e.printStackTrace();
        } finally {
            mServerSocket = null;
            mRunning = false;
        }
    }
}
