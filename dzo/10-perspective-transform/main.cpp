#include <iostream>
#include <opencv2/opencv.hpp>

void fillMatrix(double in[8][9], cv::Mat &leftT, cv::Mat &rightT) {
    leftT = cv::Mat(8, 8, CV_64F);
    rightT = cv::Mat(8, 1, CV_64F);

    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            leftT.at<double>(y,x) = in[y][x];
        }

        rightT.at<double>(y) = -in[y][8];
    }
}

void fillP3x3(cv::Mat &in, cv::Mat &out) {
    out = cv::Mat(3, 3, CV_64F);

    for (int i = 0; i < 9; i++) {
        if (i == 0) {
            out.at<double>(i) = 1;
            continue;
        }

        out.at<double>(i) = in.at<double>(i - 1);
    }
}

inline cv::Mat fillXYT(cv::Mat &in, int x, int y) {
    if (in.rows == 0) {
        in = cv::Mat(3, 1, CV_64F);
    }

    in.at<double>(0) = x;
    in.at<double>(1) = y;
    in.at<double>(2) = 1;

    return in;
}

int main() {
    cv::Mat src_8uc3_flag = cv::imread("../data/flag.png", CV_LOAD_IMAGE_COLOR);
    cv::Mat src_8uc3_vsb = cv::imread("../data/vsb.jpg", CV_LOAD_IMAGE_COLOR);
    cv::Mat dst_8uc3_result = src_8uc3_vsb.clone();

    // Points
    double transformData[8][9] = {
            {107, 1, 0,   0,   0, 0,          0,          0,    69},
            {0,   0, 69,  107, 1, 0,          0,          0,    0},
            {76,  1, 0,   0,   0, -323 * 227, -323 * 76,  -323, 227},
            {0,   0, 227, 76,  1, 0,          0,          0,    0},
            {122, 1, 0,   0,   0, -323 * 228, -323 * 122, -323, 228},
            {0,   0, 228, 122, 1, -215 * 228, -215 * 122, -215, 0},
            {134, 1, 0,   0,   0, 0,          0,          0,    66},
            {0,   0, 66,  134, 1, -215 * 66,  -215 * 134, -215, 0},
    };

    // Fill matrix
    cv::Mat leftT, rightT;
    fillMatrix(transformData, leftT, rightT);

    // Solve equation
    cv::Mat vecBuilding, vecFlag;
    cv::Mat resultP, P3x3;
    cv::solve(leftT, rightT, resultP);
    fillP3x3(resultP, P3x3);

    for (int y = 0; y < src_8uc3_vsb.rows; y++) {
        for (int x = 0; x < src_8uc3_vsb.cols; x++) {
            // Get xy1 matrix
            fillXYT(vecBuilding, x, y);

            // Solve P matrix * XY matrix
            vecFlag = P3x3 * vecBuilding;
            vecFlag /= vecFlag.at<double>(2); // divide by "z"

            // Get transformed new x and y
            int px = cv::saturate_cast<int>(vecFlag.at<double>(0));
            int py = cv::saturate_cast<int>(vecFlag.at<double>(1));

            if (px > 0 && py > 0 && px < src_8uc3_flag.cols && py < src_8uc3_flag.rows) {
                src_8uc3_vsb.at<cv::Vec3b>(y, x) = src_8uc3_flag.at<cv::Vec3b>(py, px);
            }
        }
    }

    cv::imshow("Flag", src_8uc3_flag);
    cv::imshow("VSB", src_8uc3_vsb);
    cv::waitKey(0);
    return 0;
}