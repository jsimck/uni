#include <iostream>
#include <opencv2/opencv.hpp>
#include "dft.h"

int main() {
    cv::Mat src_8uc1_lena, furier_64fc2, power_64fc1, phase_64fc1;
    cv::Mat src_64fc1_sin, furier_64fc2_sin, power_64fc1_sin, phase_64fc1_sin;

    src_8uc1_lena = cv::imread("../data/lena64.png", CV_LOAD_IMAGE_GRAYSCALE);

    // Get img size and generate sin
    int M = src_8uc1_lena.rows;
    int N = src_8uc1_lena.cols;
    src_64fc1_sin = cv::Mat(M, N, CV_64FC1);

    // Generate sin
    dft::generateSin(src_64fc1_sin);

    // Generate furier
    dft::dft(src_64fc1_sin, furier_64fc2_sin);
    dft::dft(src_8uc1_lena, furier_64fc2);

    // Generate Phase filter
    dft::dftPhase(furier_64fc2, phase_64fc1);
    dft::dftPhase(furier_64fc2_sin, phase_64fc1_sin);

    // Generate Power Filter
    dft::dftPower(furier_64fc2, power_64fc1);
    dft::dftPower(furier_64fc2_sin, power_64fc1_sin);

    // Show results
    cv::imshow("Furier", src_8uc1_lena);
    cv::imshow("Furier - Power", power_64fc1);
    cv::imshow("Furier - Phase", phase_64fc1);

    // Show results
    cv::imshow("Sin", src_64fc1_sin);
    cv::imshow("Sin - Power", power_64fc1_sin);
    cv::imshow("Sin - Phase", phase_64fc1_sin);

    cv::waitKey(0);
    return 0;
}