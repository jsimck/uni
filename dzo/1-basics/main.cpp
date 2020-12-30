#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>

int main() {
    cv::Mat src_8uc3_img;
    cv::Mat src_8uc1_img;
    cv::Mat src_32fc1_img;

    // Read image
    src_8uc3_img = cv::imread("../data/lena.png", CV_LOAD_IMAGE_COLOR);

    // Convert to grayscale and float matrix
    cv::cvtColor(src_8uc3_img, src_8uc1_img, CV_BGR2GRAY);
    src_8uc1_img.convertTo(src_32fc1_img, CV_32FC1, 1.0f / 255.0f);

    // Get and print colors at coordinates
    int y = 15, x = 10;
    uchar p1 = src_8uc1_img.at<uchar>(y, x);
    float p2 = src_32fc1_img.at<float>(y, x);
    cv::Vec3b p3 = src_8uc3_img.at<cv::Vec3b>(y, x);

    printf("Point coordinates");
    printf("\np1: %d", p1);
    printf("\np2: %f", p2);
    printf("\np3[0]: %d, p3[1]: %d, p3[2]: %d", p3[0], p3[1], p3[2]);

    // Set point at y, x to 0 (black)
    src_8uc1_img.at<uchar>(y, x) = 0;

    // Draw filled and line rectangular
    cv::rectangle(src_8uc1_img, cv::Point(30, 20), cv::Point(60, 40), cv::Scalar(20), CV_FILLED);
    cv::rectangle(src_8uc1_img, cv::Point(130, 120), cv::Point(160, 140), cv::Scalar(20), 1, cv::LINE_8);

    // Show results
    cv::imshow("8uc3", src_8uc3_img);
    cv::imshow("8uc1", src_8uc1_img);
    cv::imshow("32fc1", src_32fc1_img);

    // Draw gradients
    const int rows = 480, cols = 640;
    cv::Mat src_8uc1_gradient_single(rows, cols, CV_8UC1);
    cv::Mat src_8uc1_gradient_reverse(rows, cols, CV_8UC1);
    cv::Mat src_8uc3_gradient_color(rows, cols, CV_8UC3);

    // Black -> white gradient
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            src_8uc1_gradient_single.at<uchar>(y, x) = cv::saturate_cast<uchar>(round((255.0f / cols) * x));
        }
    }

    // White -> black gradient
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            src_8uc1_gradient_reverse.at<uchar>(y, x) = cv::saturate_cast<uchar>(255 - round((255.0f / cols) * x));
        }
    }

    // Color gradient
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            uchar nx = (uchar) round((255.0f / cols) * x);
            uchar ny = (uchar) round((255.0f / cols) * y);
            src_8uc3_gradient_color.at<cv::Vec3b>(y, x) = cv::Vec3b(ny, ny, nx);
        }
    }

    // Show gradients
    cv::imshow("Gradient Single", src_8uc1_gradient_single);
    cv::imshow("Gradient Reverse", src_8uc1_gradient_reverse);
    cv::imshow("Gradient Color", src_8uc3_gradient_color);

    // Pause program
    cv::waitKey(0);
    return 0;
}