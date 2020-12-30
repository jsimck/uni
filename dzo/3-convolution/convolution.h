#ifndef DZO_3_CONVOLUTION_H
#define DZO_3_CONVOLUTION_H

#include <opencv2/opencv.hpp>

template<typename T, int S>
void convolution(cv::Mat &src, cv::Mat &dest, int *kernel) {
    // Copy src to dest
    src.copyTo(dest);

    // Compute scale
    T scale = 0;
    for (int i = 0; i < S * S; i++) {
        scale += kernel[i];
    }

    // Loop through image using convolution mask
    int offset = S / 2;
    for (int y = offset; y < src.rows - offset; y++) {
        for (int x = offset; x < src.cols - offset; x++) {
            cv::Vec3i result = cv::Vec3i(0, 0, 0);

            int maskIndex = 0;
            for (int i = -offset; i < S - offset; i++) {
                for (int j = -offset; j < S - offset; j++) {
                    cv::Vec3i pixel = src.at<cv::Vec3b>(y + i, x + j);
                    result += pixel * kernel[maskIndex];
                    maskIndex++;
                }
            }

            if (scale == 0) {
                scale = 1;
            }

            result /= scale;
            dest.at<cv::Vec3b>(y, x) = result;
        }
    }
};

#endif