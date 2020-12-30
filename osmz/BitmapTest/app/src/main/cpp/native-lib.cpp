#include <jni.h>
#include <string>
#include <android/bitmap.h>
#include <android/log.h>
#include "Color4.h"

#define  LOG_TAG    "LibBitmap"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_kru13_bitmaptest_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_kru13_bitmaptest_MainActivity_bitmapChange(JNIEnv *env, jobject obj, jobject img, jint mode) {
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

    int height = info.height;
    int width = info.width;

    LOGI("start");
    LOGI("info height: %d", info.height);
    LOGI("info width : %d", info.width);

    uint32_t *imgp = (uint32_t *) pixels;
    int kernel[9] = { 1, 1, 1, 1, 1, 1, 1, 1, 1 };

    // Blur
    if (mode == 3) {
        for (int j = 0; j < 10; j++) {
            for (int y = 1; y < height - 1; y++) {
                for (int x = 1; x < width - 1; x++) {
                    int sumR = 0, sumG = 0, sumB = 0;
                    int kI = 0;

                    for (int yy = y - 1; yy < y + 2; yy++) {
                        for (int xx = x - 1; xx < x + 2; xx++) {
                            Color4 px(imgp[yy * width + xx]);
                            sumR += static_cast<int>(px.r) * kernel[kI];
                            sumG += static_cast<int>(px.g) * kernel[kI];
                            sumB += static_cast<int>(px.b) * kernel[kI];
                            kI++;
                        }
                    }

                    Color4 newPx(static_cast<uint32_t>(sumR / 9), static_cast<uint32_t>(sumG / 9), static_cast<uint32_t>(sumB / 9), 255);
                    imgp[y * width + x] = newPx.abgr();
                }
            }
        }
    } else {
        // Other transformations
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                Color4 px(imgp[y * width + x]);

                switch (mode) {
                    case 1:
                        px = px.grayscale();
                        break;

                    case 2:
                        px = px.inverse();
                        break;

                    default:
                        break;
                }

                imgp[y * width + x] = px.abgr();
            }
        }
    }

    AndroidBitmap_unlockPixels(env, img);
}