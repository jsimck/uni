#include <iostream>
#include <opencv2/opencv.hpp>

#define N 360

cv::Mat genInput(int rows, int cols) {
    cv::Mat input = cv::Mat::zeros(rows, cols, CV_64FC1);

    // Fill input matrix
    cv::circle(input, cv::Point(cols / 2, rows / 2), 60, 1.0, 1);
    cv::rectangle(input, cv::Rect((cols / 2), (rows / 2), 40, 40), 1.0, -1);

    return input;
}

void genProjections(cv::Mat &input, cv::Mat tpls[]) {
    // Get center point
    cv::Mat inputRotated, inputBackup;
    cv::Point2f center(input.rows / 2.0f, input.cols / 2.0f);

    for (int i = 0; i < N; i++) {
        // Get rotation matrix
        inputBackup = input.clone();
        cv::Mat rotationMat = cv::getRotationMatrix2D(center, (360 / N) * i, 1.0f);
        cv::warpAffine(inputBackup, inputRotated, rotationMat, input.size());
        tpls[i] = cv::Mat::zeros(input.rows, input.cols, CV_64FC1);

        // Create slices
        for (int y = 0; y < inputRotated.rows; y++) {
            for (int x = 0; x < inputRotated.cols; x++) {
                tpls[i].at<double>(y, 0) += inputRotated.at<double>(y, x);
            }
        }

        // Copy slices to other cols
        for (int k = 1; k < tpls[i].cols; k++) {
            tpls[i].col(0).copyTo(tpls[i].col(k));
        }
    };
}

void project(cv::Mat tpls[], cv::Mat &out) {
    // Get center point
    cv::Rect cropRect(100, 100, 400, 400);
    cv::Mat rotated, tmTpl, tmp;
    cv::Point2f center(tpls[0].rows / 2.0f, tpls[0].cols / 2.0f);
    out = cv::Mat::zeros(tpls[0].rows, tpls[0].cols, CV_64FC1);

    for (int i = 0; i < N; i++) {
        // Get rotation matrix
        cv::Mat rotationMat = cv::getRotationMatrix2D(center, -(360 / N) * i, 1.0f);
        cv::warpAffine(tpls[i], rotated, rotationMat, tpls[i].size());

        for (int y = 0; y < out.rows; y++) {
            for (int x = 0; x < out.cols; x++) {
                out.at<double>(y, x) += rotated.at<double>(y, x);
            }
        }

        // Show projections
        cv::normalize(out, tmp, 0.0, 1.0, CV_MINMAX);
        cv::normalize(rotated, tmTpl, 0.0, 1.0, CV_MINMAX);

        // Crop image
        tmp = tmp(cropRect);
        tmTpl = tmTpl(cropRect);

        // Show projections
        cv::imshow("Projected", tmp);
        cv::imshow("Projected Tpl", tmTpl);
        cv::waitKey(1);
    }

    // Normalize output
    out = out(cropRect);
    cv::normalize(out, out, 0.0, 1.0, CV_MINMAX);
}

int main() {
    cv::Mat out;
    cv::Mat input = genInput(600, 600);
    cv::Mat tpls[N];

    // Show input
    cv::imshow("Input", input(cv::Rect(100, 100, 400, 400)));
    cv::waitKey(0);

    // Generate projections
    genProjections(input, tpls);
    project(tpls, out);

    // Show results
    cv::imshow("Projected", out);

    cv::waitKey(0);
    return 0;
}