package com.example.kru13.bitmaptest;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
    private native void bitmapChange(Bitmap img, int mode);

    ImageView imgview;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        imgview = (ImageView)findViewById(R.id.imageView1);
    }

    public void buttonClick(View v) {
        Bitmap img = BitmapFactory.decodeResource(getResources(),R.drawable.vsb);
        Log.d("NDK bitmapChanged","Starting ...");

        switch (v.getId()) {
            case R.id.button1:
                // Example of a call to a native method
                bitmapChange(img, 0);
                break;

            case R.id.button2:
                // Example of a call to a native method
                bitmapChange(img, 1);
                break;

            case R.id.button3:
                // Example of a call to a native method
                bitmapChange(img, 2);
                break;

            case R.id.button4:
                // Example of a call to a native method
                bitmapChange(img, 3);
                break;
        }

        Log.d("NDK bitmapChanged","Finished");
        imgview.setImageBitmap(img);
    }

}
