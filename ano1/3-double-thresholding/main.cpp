#include <iostream>
#include <cmath>
#include <queue>
#include <utils.h>
#include <opencv2/opencv.hpp>

int t1 = 1, t2 = 5;
static cv::Mat src_8uc1, dst_64fc1;
std::string wName = "Result";

double ctan(double i) {
    if (DEG(i) == 90) return 0; // i == 90deg
    return 1.0 / std::tan(i);
}

void edgeDetection(cv::Mat &src, cv::Mat &dst, double t1 = 0.01, double t2 = 0.1) {
    // Convert source to double
    cv::Mat src_64fc1;
    src.convertTo(src_64fc1, CV_64F, 1.0 / 255.0);

    // Helpers
    cv::Mat d_64fc1 = cv::Mat::zeros(src.size(), CV_64FC1);
    cv::Mat edge_64fc1 = cv::Mat::zeros(src.size(), CV_64FC1);
    cv::Mat thresh_64fc1 = cv::Mat::zeros(src.size(), CV_64FC1);
    cv::Mat ori_64fc1 = cv::Mat::zeros(src.size(), CV_64FC1);

    // Blur input
    cv::GaussianBlur(src_64fc1, src_64fc1, cv::Size(3, 3), 0, 0);

    // Derivation
    for (int y = 1; y < src_64fc1.rows - 1; y++) {
        for (int x = 1; x < src_64fc1.cols - 1; x++) {
            // Central
            double c_nx = (src_64fc1.at<double>(y, x - 1) - src_64fc1.at<double>(y, x + 1)) / 2.0;
            double c_ny = (src_64fc1.at<double>(y - 1, x) - src_64fc1.at<double>(y + 1, x)) / 2.0;

            ori_64fc1.at<double>(y, x) = cv::fastAtan2((float) c_ny, (float) c_nx);
            d_64fc1.at<double>(y, x) = sqrt(SQR(c_nx) + SQR(c_ny));
        }
    }

    // Non-maximum suppression
    double Ep = 0.0, En = 0.0, E, alpha, oriDeg, oriRad;

    for (int y = 1; y < d_64fc1.rows - 1; y++) {
        for (int x = 1; x < d_64fc1.cols - 1; x++) {
            E = d_64fc1.at<double>(y, x);
            oriDeg = (int) ori_64fc1.at<double>(y, x) % 180;
            oriRad = RAD(oriDeg);

            // Check for oriDeg in one of 4 octans
            if (oriDeg >= 0 && oriDeg < 45) {
                alpha = std::tan(oriRad);
                Ep = alpha * d_64fc1.at<double>(y + 1, x - 1) + (1 - alpha) * d_64fc1.at<double>(y, x - 1);
                En = alpha * d_64fc1.at<double>(y - 1, x + 1) + (1 - alpha) * d_64fc1.at<double>(y, x + 1);
            } else if (oriDeg >= 45 && oriDeg < 90) {
                alpha = ctan(oriRad);
                Ep = alpha * d_64fc1.at<double>(y + 1, x - 1) + (1 - alpha) * d_64fc1.at<double>(y + 1, x);
                En = alpha * d_64fc1.at<double>(y - 1, x + 1) + (1 - alpha) * d_64fc1.at<double>(y - 1, x);
            } else if (oriDeg >= 90 && oriDeg < 135) {
                alpha = ctan(oriRad);
                Ep = alpha * d_64fc1.at<double>(y + 1, x + 1) + (1 - alpha) * d_64fc1.at<double>(y + 1, x);
                En = alpha * d_64fc1.at<double>(y - 1, x - 1) + (1 - alpha) * d_64fc1.at<double>(y - 1, x);
            } else if (oriDeg >= 135 && oriDeg < 180) {
                alpha = std::tan(oriRad);
                Ep = alpha * d_64fc1.at<double>(y + 1, x + 1) + (1 - alpha) * d_64fc1.at<double>(y, x + 1);
                En = alpha * d_64fc1.at<double>(y - 1, x - 1) + (1 - alpha) * d_64fc1.at<double>(y, x - 1);
            }

            // Check if E is maximum
            if (E > Ep && E > En) {
                edge_64fc1.at<double>(y, x) = E;
            }
        }
    }

    // Hysteresis
    std::queue<cv::Point> q;

    for (int y = 0; y < edge_64fc1.rows; y++) {
        for (int x = 0; x < edge_64fc1.cols; x++) {
            // Found edge at x,y push to queue
            if (edge_64fc1.at<double>(y, x) > t2) {
                q.push(cv::Point(x, y));

                // [Breadth first search], first look in the neighbourhood and if any neighbour is > t1 && < t2, push to queue
                // then process each x,y that's in the queue as edge and look from there for other neighbours
                while (!q.empty()) {
                    cv::Point p = q.front();
                    q.pop();

                    // Already visited
                    if (thresh_64fc1.at<double>(p) > 0.0) continue;

                    // Get all 8 neighbours coordinates [S - south, N - north, W - west, E - east]
                    cv::Point pW(p.x - 1, p.y), pNW(p.x - 1, p.y + 1), pN(p.x, p.y + 1), pNE(p.x + 1, p.y + 1);
                    cv::Point pE(p.x + 1, p.y), pSE(p.x + 1, p.y - 1), pS(p.x, p.y - 1), pSW(p.x - 1, p.y - 1);

                    // Check if pixels in neighbourhood of edge px have values > t1
                    if (edge_64fc1.at<double>(pW) >= t1 && edge_64fc1.at<double>(pW) <= t2 &&
                        thresh_64fc1.at<double>(pW) <= 0.0) {
                        q.push(pW);
                    }
                    if (edge_64fc1.at<double>(pNW) >= t1 && edge_64fc1.at<double>(pNW) <= t2 &&
                        thresh_64fc1.at<double>(pNW) <= 0.0) {
                        q.push(pNW);
                    }
                    if (edge_64fc1.at<double>(pN) >= t1 && edge_64fc1.at<double>(pN) <= t2 &&
                        thresh_64fc1.at<double>(pN) <= 0.0) {
                        q.push(pN);
                    }
                    if (edge_64fc1.at<double>(pNE) >= t1 && edge_64fc1.at<double>(pNE) <= t2 &&
                        thresh_64fc1.at<double>(pNE) <= 0.0) {
                        q.push(pNE);
                    }
                    if (edge_64fc1.at<double>(pE) >= t1 && edge_64fc1.at<double>(pE) <= t2 &&
                        thresh_64fc1.at<double>(pE) <= 0.0) {
                        q.push(pE);
                    }
                    if (edge_64fc1.at<double>(pSE) >= t1 && edge_64fc1.at<double>(pSE) <= t2 &&
                        thresh_64fc1.at<double>(pSE) <= 0.0) {
                        q.push(pSE);
                    }
                    if (edge_64fc1.at<double>(pS) >= t1 && edge_64fc1.at<double>(pS) <= t2 &&
                        thresh_64fc1.at<double>(pS) <= 0.0) {
                        q.push(pS);
                    }
                    if (edge_64fc1.at<double>(pSW) >= t1 && edge_64fc1.at<double>(pSW) <= t2 &&
                        thresh_64fc1.at<double>(pSW) <= 0.0) {
                        q.push(pSW);
                    }

                    // Set "visited"
                    thresh_64fc1.at<double>(p) = 1.0;
                }
            }
        }
    }

    // Clone result
    dst = thresh_64fc1.clone();
}

void on_change(int id, void *) {
    t1 = (t1 == 0) ? 1 : t1;
    edgeDetection(src_8uc1, dst_64fc1, t1 / 100.0, t2 / 100.0);

    if (src_8uc1.data) {
        cv::imshow(wName, dst_64fc1);
    }
}

int main() {
    // Load src
    src_8uc1 = cv::imread("data/valve.png", CV_LOAD_IMAGE_GRAYSCALE);

    // Show results
    edgeDetection(src_8uc1, dst_64fc1, t1 / 100.0, t2 / 100.0);
    cv::namedWindow(wName);
    cv::imshow(wName, dst_64fc1);

    cv::createTrackbar("t1", wName, &t1, 100, on_change);
    cv::createTrackbar("t2", wName, &t2, 100, on_change);

    cv::waitKey(0);
    return 0;
}