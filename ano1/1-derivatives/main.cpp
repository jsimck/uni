#include <iostream>
#include <cmath>
#include <utils.h>
#include <opencv2/opencv.hpp>

int main() {
    cv::Mat src_64fc1, src_8uc1, dst_64fc1_forward, dst_64fc1_backward, dst_64fc1_central,
            dst_64fc1_sobel, dst_64fc1_sobel_x, dst_64fc1_sobel_y;
    src_8uc1 = cv::imread("data/valve.png", CV_LOAD_IMAGE_GRAYSCALE);
    src_8uc1.convertTo(src_64fc1, CV_64FC1, 1.0 / 255.0);

    // Sobel masks
    int filter_sobel_x[9] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };
    int filter_sobel_y[9] = { -1, -2, -1, 0, 0, 0, 1, 2, 1 };

    // Dst images
    dst_64fc1_forward = src_64fc1.clone();
    dst_64fc1_backward = src_64fc1.clone();
    dst_64fc1_central = src_64fc1.clone();

    dst_64fc1_sobel = src_64fc1.clone();
    dst_64fc1_sobel_x = src_64fc1.clone();
    dst_64fc1_sobel_y = src_64fc1.clone();

    // Derivation
    for (int y = 1; y < src_64fc1.rows - 1; y++) {
        for (int x = 1; x < src_64fc1.cols - 1; x++) {
            // Backward
            double f_nx = src_64fc1.at<double>(y, x) - src_64fc1.at<double>(y, x - 1);
            double f_ny = src_64fc1.at<double>(y, x) - src_64fc1.at<double>(y - 1, x);

            // Forward
            double b_nx = src_64fc1.at<double>(y, x) - src_64fc1.at<double>(y, x + 1);
            double b_ny = src_64fc1.at<double>(y, x) - src_64fc1.at<double>(y + 1, x);

            // Central
            double c_nx = (src_64fc1.at<double>(y, x - 1) - src_64fc1.at<double>(y, x + 1)) / 2;
            double c_ny = (src_64fc1.at<double>(y - 1, x) - src_64fc1.at<double>(y + 1, x)) / 2;

            dst_64fc1_forward.at<double>(y, x) = sqrt(SQR(b_nx) + SQR(b_ny));
            dst_64fc1_backward.at<double>(y, x) = sqrt(SQR(f_nx) + SQR(f_ny));
            dst_64fc1_central.at<double>(y, x) = sqrt(SQR(c_nx) + SQR(c_ny));


            // Sobel
            int i = 0;
            double sumX = 0, sumY = 0;
            for (int yy = 0; yy < 3; yy++) {
                for (int xx = 0; xx < 3; xx++) {
                    sumX += src_64fc1.at<double>(y - yy + 1, x - xx + 1) * filter_sobel_x[i] * (1.0 / 8.0);
                    sumY += src_64fc1.at<double>(y - yy + 1, x - xx + 1) * filter_sobel_y[i] * (1.0 / 8.0);
                    i++;
                }
            }

            dst_64fc1_sobel_x.at<double>(y, x) = sumX;
            dst_64fc1_sobel_y.at<double>(y, x) = sumY;
            dst_64fc1_sobel.at<double>(y, x) = sqrt(SQR(sumX) + SQR(sumY));
        }
    }

    // Show results
    cv::imshow("Derivation - Forward", dst_64fc1_forward);
    cv::imshow("Derivation - Backward", dst_64fc1_backward);
    cv::imshow("Derivation - Central", dst_64fc1_central);

    cv::imshow("Sobel - X", dst_64fc1_sobel_x);
    cv::imshow("Sobel - Y", dst_64fc1_sobel_y);
    cv::imshow("Sobel", dst_64fc1_sobel);

    // Pause program
    cv::waitKey(0);
    return 0;
}