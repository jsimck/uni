#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>
#include "gaussian.h"

cv::Point gausPoint(100, 100);

void spawnGaussians(cv::Mat &src, std::vector<std::vector<std::vector<Gaussian>>> &gaussians, int N) {
    for (int y = 0; y < src.rows; y++) {
        std::vector<std::vector<Gaussian>> rowGaussians;

        for (int x = 0; x < src.cols; x++) {
            std::vector<Gaussian> pointGaussians;

            for (int i = 0; i < N; i++) {
//                double u = ((256 / N) * (i + 1)) - (256 / N) / 2;
//                pointGaussians.push_back(Gaussian(u, 25, 1.0 / N));
                double ur = (rand() % (255 - 30)) + 30;
                double sdr = (rand() % (30 - 10)) + 10;
                pointGaussians.push_back(Gaussian(ur, sdr, 1.0 / N));
            }

            rowGaussians.push_back(pointGaussians);
        }

        gaussians.push_back(rowGaussians);
    }
}

void updateGaussians(cv::Mat &src, cv::Mat &detection, std::vector<std::vector<std::vector<Gaussian>>> &gaussians) {
    #pragma omp parallel for
    for (int y = 0; y < src.rows; y++) {
        for (int x = 0; x < src.cols; x++) {
            double px = static_cast<double>(src.at<uchar>(y, x));
            double sumP = Gaussian::sumProbability(px, gaussians[y][x]);

            int maxI = 0;
            int i = 0;
            double maxP = 0;
            for (auto &gaus : gaussians[y][x]) {
                gaus.update(px, sumP, 0.1, 4);

                // Find max
                if (gaus.p > maxP) {
                    maxP = gaus.p;
                    maxI = i;
                }

                i++;
            }
//            maxI = Gaussian::max(px, gaussians[y][x]);
//            gaussians[y][x][maxI].update(px, sumP, 0.1, 5);

            // Detect changes
            Gaussian &gaus = gaussians[y][x][maxI];
            double minPx = gaus.u - (gaus.sd * 2);
            double maxPx = gaus.u + (gaus.sd * 2);

            if (px > minPx && px < maxPx) {
                detection.at<uchar>(y, x) = 0; // background
            } else {
                detection.at<uchar>(y, x) = 255; // change
            }
        }
    }
}

void CallBackFunc(int event, int x, int y, int flags, void *userdata) {
    if (event == CV_EVENT_LBUTTONDOWN) {
        gausPoint.x = x;
        gausPoint.y = y;
    }
}

int main() {
    const int N = 3;
    int iter = 0;
    cv::Mat frame, frameBGR, graph;
    std::vector<std::vector<std::vector<Gaussian>>> gaussians;

    // Load video
	cv::VideoCapture capture("data/dt_passat.mpg");
    assert(capture.isOpened());

    // Load first frame to init matrix
    capture >> frame;
    cv::Mat detection = cv::Mat::zeros(frame.size(), CV_8UC1);

    while (true) {
        capture >> frame;
        cv::cvtColor(frame, frame, CV_BGR2GRAY);

        // Spawn gaussians on first loop and update in the rest of the loop
        if (iter <= 0) {
            spawnGaussians(frame, gaussians, N);
        } else {
            updateGaussians(frame, detection, gaussians);
        }

        // Visualise graph
        Gaussian::visualize(graph, gaussians, gausPoint, frame.at<uchar>(gausPoint));

        // Show fps and dot in place of gaussian visualisation
        cv::cvtColor(frame, frameBGR, CV_GRAY2BGR);
        cv::circle(frameBGR, gausPoint, 2, cv::Vec3b(0, 255, 0), -1);
        cv::putText(frameBGR, std::to_string(iter), cv::Point(30, 30), CV_FONT_HERSHEY_DUPLEX, 1, cv::Vec3b(0, 255, 0));

        // Show results
        cv::imshow("Detection", detection);
        cv::imshow("Gaussian", graph);
        cv::namedWindow("Video", 1);
        cv::setMouseCallback("Video", CallBackFunc, NULL);
        cv::imshow("Video", frameBGR);
        cv::waitKey(1);

        // Up iter count
        iter++;
    }

    // Cleanup
    capture.release();
    cv::destroyAllWindows();
    return 0;
}