#include <jni.h>
#include <string>
#include <cmath>
#include <android/bitmap.h>
#include <android/log.h>
#include "Color4.h"
#include "Range.h"
#include "Size.h"
#include "Complex.h"

#define SQR(x) ((x) * (x))
#define  LOG_TAG    "LibBitmap"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_kru13_fractal_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_kru13_fractal_MainActivity_fractal(JNIEnv *env, jobject obj, jobject img,
                                                    jdouble minR, jdouble maxR, jdouble minI,
                                                    jdouble maxI) {
    AndroidBitmapInfo info;
    void *pixels;
    int ret;
    static int init;

    if ((ret = AndroidBitmap_getInfo(env, img, &info)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return;
    }

    if ((ret = AndroidBitmap_lockPixels(env, img, &pixels)) < 0) {
        LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
    }

    if (info.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGE("Bitmap format is not RGBA_8888 !");
        return;
    }

    uint32_t **raw_data = (uint32_t **) pixels;

    LOGI("start");
    LOGI("info height: %d", info.height);
    LOGI("info width : %d", info.width);

    uint32_t *imgp = (uint32_t *) pixels;

    // Fractal params
    Size<double> size(static_cast<double>(info.width), static_cast<double>(info.height));
    Range<double> R(minR, maxR);
    Range<double> I(minI, maxI);
    double Rf = (R.max - R.min) / (size.width - 1);
    double If = (I.max - I.min) / (size.height - 1);
    const int it = 200;

    for (int y = 0; y < size.height; y++) {
        double nI = I.max - y * If;

        for (int x = 0; x < size.width; x++) {
            double nR = R.min + x * Rf;
            Complex<double> C(nR, nI);
            Complex<double> Z(nR, nI);

            for (int n = 0; n < it; ++n) {
                double zR2 = SQR(Z.r);
                double zI2 = SQR(Z.i);
                Color4 newPx;

                if (zR2 + zI2 > 4) {
                    newPx.b = 255 - std::log(n) * 100.0;    //color[1] = 255 - log(n)*100.0;
                    imgp[y * static_cast<int>(size.width) + x] = newPx.abgr();
                    break;
                }

                // Z_n+1 = Z^2 + c
                Z = SQR(Z) + C;
            }
        }
    }

    AndroidBitmap_unlockPixels(env, img);
}