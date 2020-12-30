#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>

int main() {
    cv::Mat src_8uc1_img;
    cv::Mat src_8uc1_img_contrast;
    cv::Mat src_32fc1_img_gamma;

    src_8uc1_img = cv::imread("../data/moon.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    src_8uc1_img.convertTo(src_32fc1_img_gamma, CV_32FC1, 1.0 / 255.0);
    src_8uc1_img.copyTo(src_8uc1_img_contrast);

    const int cols = src_8uc1_img.cols, rows = src_8uc1_img.rows;
    const float gammaCoeff = 0.8; // <0.5, 2.5>

    // Gamma correction
    // Iout = Iin^c ; c => <0.5, 2.5>
    // Principle -> work with float format 32f => 100^2 = 10000 vs 0.39^2 = 0.152
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            src_32fc1_img_gamma.at<float>(y, x) = powf(src_32fc1_img_gamma.at<float>(y, x), gammaCoeff);
        }
    }

    // Get max and min color value
    double min, max;
    cv::minMaxLoc(src_8uc1_img_contrast, &min, &max);
    float contrastCoeff = 255 / (float) (max - min);

    // Contrast enhancements
    // Iout = round((Iin - min) * (255 / delta)) ; delta => max - min
    // Principle -> stretch range of colors to <0, 255> range, in our case <1, 160> => <0, 255>
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            uchar minPointVal = src_8uc1_img_contrast.at<uchar>(y, x) - (uchar) min;
            src_8uc1_img_contrast.at<uchar>(y, x) = cv::saturate_cast<uchar>(roundf(minPointVal * contrastCoeff));
        }
    }

    cv::imshow("Original", src_8uc1_img);
    cv::imshow("Gamma", src_32fc1_img_gamma);
    cv::imshow("Contrast", src_8uc1_img_contrast);
    cv::waitKey(0);

    return 0;
}