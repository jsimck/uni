package com.example.kru13.md5;

import android.graphics.Bitmap;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Base64;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;

import java.math.BigInteger;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Objects;

public class MainActivity extends AppCompatActivity {
    public static final String TAG = "MainActivity";

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
    private native void md5(String msg);

    TextView tvJava;
    TextView tvCpp;
    TextView tvHash;
    EditText teEditText;
    KeyGenerator mKeyGenerator;
    MessageDigest md;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);

        try {
            md = MessageDigest.getInstance("MD5");
        } catch (NoSuchAlgorithmException e) {
            Log.e("MD5", e.getLocalizedMessage());
        }

        tvJava = (TextView) findViewById(R.id.javaTextView);
        tvCpp = (TextView) findViewById(R.id.cppTextView);
        tvHash = (TextView) findViewById(R.id.hashTV);
        teEditText = (EditText) findViewById(R.id.md5edittext);

        mKeyGenerator = new KeyGenerator();
    }

    public void buttonClick(View v) {
        switch (v.getId()) {
            case R.id.button1:
                crackMD5();
                break;
        }
    }

    public String getMd5Hash(String input) {
        byte[] messageDigest = md.digest(input.getBytes());
        BigInteger number = new BigInteger(1, messageDigest);
        String md5 = number.toString(16);
        while (md5.length() < 32) {
            md5 = "0" + md5;
        }
        return md5;
    }

    public void crackMD5() {
        String md5Str = getMd5Hash(teEditText.getText().toString());
        tvHash.setText("HASH: " + md5Str);

        // Run CPP
        Log.d("MD5 Cracking", "Starting CPP... " + md5Str);
        long startTime = System.currentTimeMillis();
        md5(md5Str);
        long stopTime = System.currentTimeMillis();
        long cppElapsedTime = stopTime - startTime;
        Log.d("MD5 Cracking", "CPP Finished: " + cppElapsedTime + "ms");

        // Run Java
        Log.d("MD5 Cracking", "Starting Java... " + md5Str);
        startTime = System.currentTimeMillis();
        while(!Objects.equals(md5Str, getMd5Hash(mKeyGenerator.generate())));
        stopTime = System.currentTimeMillis();
        long javaElapsedTime = stopTime - startTime;
        Log.d("MD5 Cracking", "Java Finished: " + javaElapsedTime + "ms");

        tvJava.setText("Java time: " + cppElapsedTime + "ms");
        tvCpp.setText("CPP time: " + javaElapsedTime + "ms");

        Log.d("MD5 Cracking", "Finished");
    }
}
