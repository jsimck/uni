#include <iostream>
#include <opencv2/opencv.hpp>
#include "dft.h"

int main() {
    cv::Mat src_8uc1_lena, furier_64fc2, power_64fc1, phase_64fc1;
    cv::Mat src_64fc1_sin, furier_64fc2_sin, power_64fc1_sin, phase_64fc1_sin;
    cv::Mat inverse_64fc1;

    src_8uc1_lena = cv::imread("../data/lena64.png", CV_LOAD_IMAGE_GRAYSCALE);

    // Get img size and generate sin
    int M = src_8uc1_lena.rows;
    int N = src_8uc1_lena.cols;
    src_64fc1_sin = cv::Mat(M, N, CV_64FC1);

    // Furier
    dft::dft(src_8uc1_lena, furier_64fc2);

    // Inverse
    dft::idft(furier_64fc2, inverse_64fc1);

    cv::imshow("Inverse furier", inverse_64fc1);

    cv::Mat circle = dft::circleMask(64, 64, 26);
    cv::imshow("Circle", circle);

    cv::waitKey(0);
    return 0;
}