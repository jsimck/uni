#include <opencv2/core/mat.hpp>
#include <opencv2/core/persistence.hpp>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <random>
#include "e2_flow.h"

void E2Flow::loadData() {
    // Load matrices
    std::ostringstream oss;
    std::string fileName = "u00195.yml";
    cv::FileStorage fs;

    for (int i = 0; i < 200; i++) {
        // Build filename
        oss.str("");
        oss << "flow/u" << std::setw(5) << std::setfill('0') << i << ".yml";

        // Load matrix
        cv::Mat flow;
        fs.open(oss.str().c_str(), cv::FileStorage::Mode::READ | cv::FileStorage::Mode::FORMAT_AUTO);
        fs["flow"] >> flow;
        flows.emplace_back(flow);
        fs.release();
    }
}

int E2Flow::run(int argc, char **argv) {
    // Load matrices
    loadData();

    // Init data
    cv::Size flowSize = flows[0].size();
    lines = cv::Mat::zeros(flowSize.height * scale, flowSize.width * scale, CV_8UC3);

    // Init random points
    int offset = flowSize.width / numPoints;
    for (int i = 0; i <= numPoints; ++i) {
        points.emplace_back((offset / 2) + offset * i, flowSize.height - 20);
    }

    // Show vectors for each matrix
    for (auto &flow : flows) {
        visualizeFlow(flow, result);
        cv::imshow("Flow", result);
        cv::waitKey(1);
    }
    cv::waitKey(0);

    return 0;
}

cv::Point2f E2Flow::getSubPixel(const cv::Mat &flow, cv::Point2f pt) {
    cv::Mat patch;
    cv::remap(flow, patch, cv::Mat(1, 1, CV_32FC2, &pt), cv::noArray(),
              cv::INTER_LINEAR, cv::BORDER_REFLECT_101);
    cv::Vec2f v = patch.at<cv::Vec2f>(0, 0);

    return cv::Point2f(v[0], v[1]);
}

cv::Point2f E2Flow::rungeKutta(const cv::Mat &flow, cv::Point2f p, float dt) {
    cv::Point2f K1 = getSubPixel(flow, p) * dt;
    cv::Point2f K2 = getSubPixel(flow, p + K1 * 0.5f) * dt;
    cv::Point2f K3 = getSubPixel(flow, p + K2 * 0.5f) * dt;
    cv::Point2f K4 = getSubPixel(flow, p + K3) * dt;

    return p + ((1.0f / 6.0f) * (K1 + 2 * K2 + 2 * K3 + K4));
}

std::vector<cv::Point2f> E2Flow::movePoints(const cv::Mat &flow, std::vector<cv::Point2f> &points, float dt) {
    std::vector<cv::Point2f> nPoints = points;

    for (auto &point : nPoints) {
        point = rungeKutta(flow, point, dt);
    }

    return nPoints;
}

void E2Flow::computeCurl(const cv::Mat &flow, cv::Mat &curl, cv::Mat &color) {
    curl = cv::Mat::zeros(flow.size(), CV_32FC1);

    for (int y = 1; y < flow.rows - 1; y++) {
        for (int x = 1; x < flow.cols - 1; x++) {
            float dy = flow.at<cv::Point2f>(y - 1, x).x - flow.at<cv::Point2f>(y + 1, x).x;
            float dx = flow.at<cv::Point2f>(y, x - 1).y - flow.at<cv::Point2f>(y, x + 1).y;
            curl.at<float>(y, x) = dx - dy;
        }
    }

    // Apply color map
    cv::normalize(curl, color, 0, 255, CV_MINMAX);
    color.convertTo(color, CV_8UC1);
    applyColorMap(color, color, cv::COLORMAP_JET);
}

void E2Flow::visualizeFlow(const cv::Mat &flow, cv::Mat &scene) {
    // Compute rotor, vorticity and scale
    cv::Mat color, curl;
    computeCurl(flow, curl, scene);
    cv::resize(scene, scene, cv::Size(), scale, scale, CV_INTER_CUBIC);

    double min, max;
    cv::minMaxLoc(flow, &min, &max);
    auto arrowScale = static_cast<float>(MAX(abs(min), abs(max)) / scale);

    // Draw arrows
    for (int y = 0; y < flow.rows; y++) {
        for (int x = 0; x < flow.cols; x++) {
            auto vec = flow.at<cv::Point2f>(y, x) * arrowScale;

            cv::Point2f pos((x + 0.5f) * scale, (y + 0.5f) * scale);
            auto start = pos - vec;
            auto end = pos + vec;

            cv::arrowedLine(scene, start, end, cv::Scalar::all(0), 1, 8, 0, 0.3);
        }
    }

    // Move points
    for (int i = 0; i < iterations; i++) {
        auto nPoints = movePoints(flow, points, dt);

        for (size_t j = 0; j < points.size(); j++) {
            auto nPoint = nPoints[j] * (scale);
            auto point = points[j] * (scale);
            cv::line(lines, point, nPoint, cv::Scalar(0, 0, 255), 2, CV_AA);
        }

        points.swap(nPoints);
    }

    cv::add(scene, lines, scene);
}
