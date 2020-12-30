package com.example.kru13.fractal;

import android.graphics.Bitmap;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;

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

    private native void fractal(Bitmap img, double minR, double maxR, double minI, double maxI);

    int width = 800;
    int height = 600;

    double offsetDownX = 0;
    double offsetUpX = 0;
    double offsetDownY = 0;
    double offsetUpY = 0;

    double minR = -2.2;
    double maxR = 1.0;
    double minI = -1.2;
    double maxI = 1.2;

    ImageView imgview;
    TextView tvMinR;
    TextView tvMaxR;
    TextView tvMinI;
    TextView tvMaxI;

    Bitmap.Config mConf;
    Bitmap mB;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        mConf = Bitmap.Config.ARGB_8888;
        mB = Bitmap.createBitmap(width, height, mConf);
        setContentView(R.layout.activity_main);

        imgview = (ImageView) findViewById(R.id.imageView1);
        tvMinR = (TextView) findViewById(R.id.minR);
        tvMaxR = (TextView) findViewById(R.id.maxR);
        tvMinI = (TextView) findViewById(R.id.minI);
        tvMaxI = (TextView) findViewById(R.id.maxI);

        imgview.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {

                if (motionEvent.getAction() == android.view.MotionEvent.ACTION_DOWN) {
                    offsetDownX = motionEvent.getX();
                    offsetDownY = motionEvent.getY();
                } else if (motionEvent.getAction() == android.view.MotionEvent.ACTION_UP) {
                    offsetUpX = motionEvent.getX();
                    offsetUpY = motionEvent.getY();

                    // Calculate new intervals
                    double intR = maxR - minR;
                    double intI = maxI - minI;

                    double Rf = intR / width;
                    double If = intI / height;

                    double omaxR = maxR;
                    double ominI = minI;
                    double omaxI = maxI;
                    double ominR = minR;

                    minR = (offsetDownX * Rf) + ominR;
                    maxR = (offsetUpX * Rf) + ominR;

                    minI = (offsetUpY * If) + ominI;
                    maxI = (offsetDownY * If) + ominI;

                    calcFractal();
                }

                return true;
            }
        });

        calcFractal();
    }

    public void buttonClick(View v) {
        switch (v.getId()) {
            case R.id.button1:
                // Example of a call to a native method
                minR = -2.2;
                maxR = 1.0;
                minI = -1.2;
                maxI = 1.2;
                calcFractal();
                break;
        }
    }

    void calcFractal() {
        Log.d("NDK fractal", "Starting ...");

        mB = Bitmap.createBitmap(width, height, mConf);
        fractal(mB, minR, maxR, minI, maxI);
        imgview.setImageBitmap(mB);

        tvMinR.setText("minR: " + minR);
        tvMaxR.setText("maxR: " + maxR);
        tvMinI.setText("minI: " + minI);
        tvMaxI.setText("maxI: " + maxI);

        Log.d("NDK fractal", "Finished");
    }
}
