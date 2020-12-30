#include <iostream>
#include <opencv2/opencv.hpp>

void laplace(cv::Mat &src) {
    cv::Mat src_64fc1_gaussian, src_64fc1;
    cv::Mat dst_64fc1_laplace, dst_64fc1_laplace_gaus, dst_8uc3_laplace_color;
    src.convertTo(src_64fc1, CV_64FC1, 1.0 / 255.0);

    // Apply blur
    cv::GaussianBlur(src_64fc1, src_64fc1_gaussian, cv::Size(3, 3), 0, 0);

    // Sobel masks
    int filterLaplace[9] = { 0, 1, 0, 1, -4, 1, 0, 1, 0 };

    // Dst images
    dst_64fc1_laplace = cv::Mat(src.rows, src.cols, CV_64FC1);
    dst_64fc1_laplace_gaus = cv::Mat(src.rows, src.cols, CV_64FC1);
    dst_8uc3_laplace_color = cv::Mat(src.rows, src.cols, CV_8UC3);

    // Derivation
    for (int y = 1; y < src_64fc1.rows - 1; y++) {
        for (int x = 1; x < src_64fc1.cols - 1; x++) {

            // Laplace
            int i = 0;
            double sum = 0, sumGaus = 0;
            for (int yy = 0; yy < 3; yy++) {
                for (int xx = 0; xx < 3; xx++) {
                    sum += src_64fc1.at<double>(y - yy + 1, x - xx + 1) * filterLaplace[i];
                    sumGaus += src_64fc1_gaussian.at<double>(y - yy + 1, x - xx + 1) * filterLaplace[i];
                    i++;
                }
            }

            cv::Vec3b color(0, 0, 0);
            if (sumGaus < 0) {
                color = cv::Vec3b(0, 0, 255);
            } else if (sumGaus > 0) {
                color = cv::Vec3b(0, 255, 0);
            }

            dst_8uc3_laplace_color.at<cv::Vec3b>(y, x) = color;
            dst_64fc1_laplace.at<double>(y, x) = sum;
            dst_64fc1_laplace_gaus.at<double>(y, x) = sumGaus;
        }
    }

    // Show results
    cv::imshow("Laplace - Gaus", dst_64fc1_laplace_gaus);
    cv::imshow("Laplace - Color", dst_8uc3_laplace_color);
    cv::imshow("Laplace", dst_64fc1_laplace);

    // Pause program
    cv::waitKey(0);
}

int main() {
    cv::Mat src_8uc1;
    src_8uc1 = cv::imread("data/valve.png", CV_LOAD_IMAGE_GRAYSCALE);

    // Laplace
    laplace(src_8uc1);

    // Pause program
    cv::waitKey(0);
    return 0;
}