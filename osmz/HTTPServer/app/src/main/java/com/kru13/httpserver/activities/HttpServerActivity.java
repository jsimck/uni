package com.kru13.httpserver.activities;

import android.Manifest;
import android.annotation.TargetApi;
import android.app.ActivityManager;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Color;
import android.os.Build;
import android.os.Bundle;
import android.app.Activity;
import android.os.Handler;
import android.os.Message;
import android.support.annotation.NonNull;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;

import com.kru13.httpserver.R;
import com.kru13.httpserver.services.HttpBackgroundService;
import com.kru13.httpserver.threads.SocketServer;
import com.kru13.httpserver.handlers.CameraHandler;
import com.kru13.httpserver.listeners.CameraCaptureListener;
import com.kru13.httpserver.models.Payload;

public class HttpServerActivity extends Activity implements OnClickListener {
    private static final String TAG = "HttpServerActivity";

    private static final int PERMISSIONS_ID = 1;
    private static final String PERMISSIONS[] = {
        Manifest.permission.CAMERA,
        Manifest.permission.INTERNET,
        Manifest.permission.READ_EXTERNAL_STORAGE,
        Manifest.permission.WRITE_EXTERNAL_STORAGE,
    };

    private Intent mServiceIntent;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // Ask for permissions
        if (shouldAskPermissions()) {
            askPermissions();
        }

        // Init intent
        mServiceIntent = new Intent(this, HttpBackgroundService.class);

        setContentView(R.layout.activity_http_server);
        Button btn1 = (Button) findViewById(R.id.button1);
        Button btn2 = (Button) findViewById(R.id.button2);

        btn2.setTextColor(Color.rgb(244, 67, 54));
        btn1.setOnClickListener(this);
        btn2.setOnClickListener(this);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.http_server, menu);
        return true;
    }

    @Override
    protected void onStop() {
        super.onStop();
    }

    @Override
    public void onClick(View v) {
        if (v.getId() == R.id.button1) {
            startService(mServiceIntent);

            // Change button title and bg
            ((Button) v).setText("HTTP Server running");
            ((Button) v).setTextColor(Color.rgb(76, 175, 80));
        }

        if (v.getId() == R.id.button2 && mServiceIntent != null) {
            stopService(mServiceIntent);
        }
    }

    protected boolean shouldAskPermissions() {
        return (Build.VERSION.SDK_INT > Build.VERSION_CODES.LOLLIPOP_MR1);
    }

    @TargetApi(23)
    protected void askPermissions() {
        for (String permission : PERMISSIONS) {
            if (ContextCompat.checkSelfPermission(this, permission) != PackageManager.PERMISSION_GRANTED) {
                ActivityCompat.requestPermissions(this, PERMISSIONS, PERMISSIONS_ID);
            }
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        switch (requestCode) {
            case PERMISSIONS_ID: {
                // If request is cancelled, the result arrays are empty.
                if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                    // TODO - permission was granted, send service info to reopen camera
                }
            }
        }
    }
}
