#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/saliency.hpp>
#include "dft.h"

int main() {
    cv::Mat src_8uc1_lena_noise, lena_noise_64fc1_cleared;
    cv::Mat src_8uc1_lena_noise_2, lena_noise_2_64fc1_cleared;
    cv::Mat src_8uc1_lena_bars, lena_bars_64fc1_cleared;
    cv::Mat filter_bars, filter_lowPass, filter_highPass;

    // Load images
    src_8uc1_lena_noise = cv::imread("../data/lena64_noise.png", CV_LOAD_IMAGE_GRAYSCALE);
    src_8uc1_lena_noise_2 = cv::imread("../data/lena64_noise.png", CV_LOAD_IMAGE_GRAYSCALE);
    src_8uc1_lena_bars = cv::imread("../data/lena64_bars.png", CV_LOAD_IMAGE_GRAYSCALE);

    // Get source rows and cols
    int rows = src_8uc1_lena_noise.rows;
    int cols = src_8uc1_lena_noise.cols;

    // Create Filters
    filter_lowPass = cv::Mat::zeros(64, 64, CV_8UC1);
    filter_bars = cv::Mat::ones(64, 64, CV_8UC1) * 255;
    filter_highPass = cv::Mat::ones(64, 64, CV_8UC1) * 255;

    cv::circle(filter_lowPass, cv::Point(rows / 2, cols / 2), 20, 255, -1);
    cv::circle(filter_highPass, cv::Point(rows / 2, cols / 2), 8, 0, -1);

    // Bars filter
//    cv::circle(filter_bars, cv::Point(rows / 2, cols / 2), 28, 255, -1);

    for (int i = 0; i < 64; i++) {
        // We skip center of image
        if (i > 30 && i < 34) continue;
        filter_bars.at<uchar>(32, i) = 0;
    }

    // Apply filtering
    dft::frequencyFilter(src_8uc1_lena_noise, filter_lowPass, lena_noise_64fc1_cleared);
    dft::frequencyFilter(src_8uc1_lena_noise_2, filter_highPass, lena_noise_2_64fc1_cleared);
    dft::frequencyFilter(src_8uc1_lena_bars, filter_bars, lena_bars_64fc1_cleared);

    // Resize so changes are more visible
    cv::resize(lena_noise_64fc1_cleared, lena_noise_64fc1_cleared, cv::Size(), 3, 3);
    cv::resize(lena_noise_2_64fc1_cleared, lena_noise_2_64fc1_cleared, cv::Size(), 3, 3);
    cv::resize(lena_bars_64fc1_cleared, lena_bars_64fc1_cleared, cv::Size(), 3, 3);

    // Resize filters
    cv::resize(filter_lowPass, filter_lowPass, cv::Size(), 3, 3);
    cv::resize(filter_highPass, filter_highPass, cv::Size(), 3, 3);
    cv::resize(filter_bars, filter_bars, cv::Size(), 3, 3);

    // Show results
    cv::imshow("Low Pass", lena_noise_64fc1_cleared);
    cv::imshow("High Pass", lena_noise_2_64fc1_cleared);
    cv::imshow("Bars", lena_bars_64fc1_cleared);

    // Show Filter results
    cv::imshow("Low Pass filter", filter_lowPass);
    cv::imshow("High Pass filter", filter_highPass);
    cv::imshow("Bars filter", filter_bars);

    cv::waitKey(0);
    return 0;
}