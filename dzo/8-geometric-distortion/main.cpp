#include <stdio.h>
#include <stdlib.h>
#include <cv.h>
#include <cvaux.h>
#include <opencv2/opencv.hpp>

#define SQR(x) ( ( x ) * ( x ) )
#define DEG2RAD(x) ( ( x ) * M_PI / 180.0 )
#define MY_MIN(X, Y) ( ( X ) < ( Y ) ? ( X ) : ( Y ) )
#define MY_MAX(X, Y) ( ( X ) > ( Y ) ? ( X ) : ( Y ) )

int K1 = 3, K2 = 1;
static cv::Mat img, img_geom;

cv::Vec3b bilinearInterpolation(cv::Mat &src, cv::Point2d point) {
    // Get adjacent pixel coordinates
    double x1 = floor(point.x);
    double y1 = floor(point.y);
    double x2 = ceil(point.x);
    double y2 = ceil(point.y);

    // Get adjacent pixel values
    cv::Vec3b f_00 = src.at<cv::Vec3b>(y1, x1);
    cv::Vec3b f_01 = src.at<cv::Vec3b>(y1, x2);
    cv::Vec3b f_10 = src.at<cv::Vec3b>(y2, x1);
    cv::Vec3b f_11 = src.at<cv::Vec3b>(y2, x2);

    // Move x and y into <0,1> square
    double x = point.x - x1;
    double y = point.y - y1;

    // Interpolate
    return (f_00 * (1 - x) * (1 - y)) + (f_01 * x * (1 - y)) + (f_10 * (1 - x) * y) + (f_11 * x * y);
}

void geom_dist(cv::Mat &src, cv::Mat &dst, bool bili, double K1 = 1.0, double K2 = 1.0) {
    // Center of source image and R
    int cu = src.cols / 2;
    int cv = src.rows / 2;
    double R = sqrt(SQR(cu) + SQR(cv));

    for (int y_n = 0; y_n < src.rows; y_n++) {
        for (int x_n = 0; x_n < src.cols; x_n++) {
            double x_ = (x_n - cu) / R;
            double y_ = (y_n - cv) / R;
            double r_2 = SQR(x_) + SQR(y_);
            double theta = 1 + K1 * r_2 + K2 * SQR(r_2);

            double x_d = (x_n - cu) * (1 / theta) + cu;
            double y_d = (y_n - cv) * (1 / theta) + cv;

            // Interpolate
            dst.at<cv::Vec3b>(y_n, x_n) = bili ? bilinearInterpolation(src, cv::Point2d(x_d, y_d)) : src.at<cv::Vec3b>(y_d, x_d);
        }
    }
}

void on_change(int id, void*) {
    geom_dist(img, img_geom, true, K1 / 100.0, K2 / 100.0);
    if (img_geom.data) {
        cv::imshow("Geom Dist", img_geom);
    }
}

int main() {
    img = cv::imread("../data/distorted_window.jpg", CV_LOAD_IMAGE_COLOR);

    if (!img.data) {
        printf("Unable to load image!\n");
        exit(-1);
    }

    std::string windowNameOriginal = "Original Image";
    cv::namedWindow(windowNameOriginal);
    cv::imshow(windowNameOriginal, img);

    img.copyTo(img_geom);
    geom_dist(img, img_geom, true, K1 / 100.0, K2 / 100.0);

    std::string windowNameDist = "Geom Dist";
    cv::namedWindow(windowNameDist);
    cv::imshow(windowNameDist, img_geom);

    cv::createTrackbar("K1", windowNameDist, &K1, 1000, on_change);
    cv::createTrackbar("K2", windowNameDist, &K2, 1000, on_change);

    cv::waitKey(0);
    return 0;
}
