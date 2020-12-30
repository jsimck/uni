#include "gaussian.h"
#include <cmath>
#include <utils.h>
#include <cassert>
#include <opencv2/opencv.hpp>

double Gaussian::calcProbability(double X, Gaussian &g) {
    double e = std::exp(-((SQR(X - g.u) / (2.0 * SQR(g.sd)))));
    double p = (1.0 / (g.sd * std::sqrt(2.0 * M_PI))) * e;

    return p;
}

double Gaussian::sumProbability(double X, std::vector<Gaussian> &gaussians) {
    double sum = 0;

    for (auto g : gaussians) {
        sum += g.p * calcProbability(X, g);
    }

    return sum;
}

double Gaussian::conditionalProbability(double X, Gaussian &g, double sumP) {
    return (g.p * calcProbability(X, g)) / sumP;
}

void Gaussian::update(double X, double sumP, double alpha, double minSD) {
    // Probabilities
    double alphaP = alpha * conditionalProbability(X, *this, sumP);
    double newP = (1.0 - alpha) * this->p + alphaP;
    double ro = alphaP / newP;

    // SD and mean
    double newU = (1 - ro) * this->u + ro * X;
    double newSD = std::sqrt((1 - ro) * SQR(this->sd) + ro * SQR(X - newU));

    // Update new calculated values
    this->u = newU;
    this->sd = (newSD < minSD) ? minSD : newSD;
    this->p = newP;
}

int Gaussian::max(double X, std::vector<Gaussian> &gaussians) {
    double maxP = 0;
    int maxIndex = -1;

    for (int i = 0; i < gaussians.size(); i++) {
        double P = conditionalProbability(X, gaussians[i]);
        if (P > maxP) {
            maxP = P;
            maxIndex = i;
        }
    }

    return maxIndex;
}

void Gaussian::visualize(cv::Mat &dst, std::vector<std::vector<std::vector<Gaussian>>> &gaussians, cv::Point &center, int x) {
    // Init variables
    dst = cv::Mat::zeros(256, 256, CV_8UC3);
    std::vector<std::vector<cv::Point_<double>>> graphsPoints;
    double maxY = 0;

    // Gaussian colors
    cv::Vec3b colors[5] = {
        cv::Vec3b(244, 67, 54),
        cv::Vec3b(103, 58, 183),
        cv::Vec3b(33, 150, 243),
        cv::Vec3b(76, 175, 80),
        cv::Vec3b(63, 81, 181),
    };

    // Find maxY and extract gaussian points
    for (int x = 0; x < dst.cols - 1; x++) {
        std::vector<cv::Point_<double>> graphsPoint;

        // Extract points for each gaussian
        for (int i = 0; i < gaussians[center.y][center.x].size(); i++) {
            double y = Gaussian::calcProbability(x, gaussians[center.y][center.x][i]);
            graphsPoint.push_back(cv::Point_<double>(x, y));

            // Find maxY
            if (y > maxY) {
                maxY = y;
            }
        }

        graphsPoints.push_back(graphsPoint);
    }

    double offset = 240 / maxY;

    // Draw gaussian points with given offset
    for (int i = 1; i < graphsPoints.size(); i++) {
        for (int j = 0; j < graphsPoints[i].size(); j++) {
            cv::Point p1(
                static_cast<int>(graphsPoints[i - 1][j].x),
                static_cast<int>(255 - offset * graphsPoints[i - 1][j].y)
            );
            cv::Point p2(
                static_cast<int>(graphsPoints[i][j].x),
                static_cast<int>(255 - offset * graphsPoints[i][j].y)
            );

            // Draw line between 2 points
            cv::line(dst, p1, p2, colors[j % 5], 1);
        }
    }

    // Draw line where the pixel value is located now
    cv::line(dst, cv::Point(x, 0), cv::Point(x, dst.rows), cv::Vec3b(0, 255, 0), 2);
    cv::putText(dst, std::to_string(x), cv::Point(15, 30), CV_FONT_HERSHEY_SIMPLEX, 0.5, cv::Vec3b(255, 255, 255));
}

std::ostream &operator<<(std::ostream &os, const Gaussian &gaussian) {
    os << "sd: " << static_cast<int>(gaussian.sd) << " u: " << static_cast<int>(gaussian.u) << " p: " << gaussian.p << " <" << (gaussian.u - 3 * gaussian.sd) << ", " << (gaussian.u + 3 * gaussian.sd) << ">";
    return os;
}
