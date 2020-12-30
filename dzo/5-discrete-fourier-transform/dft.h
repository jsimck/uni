#ifndef DZO_5_DFT_H
#define DZO_5_DFT_H

#include <opencv2/opencv.hpp>

#define SQR(x) x * x

namespace dft {
    void dft(cv::Mat &source, cv::Mat &furier);
    void dftPhase(cv::Mat &furier, cv::Mat &dest);
    void dftPower(cv::Mat &furier, cv::Mat &dest);
    void flipQuadrants(cv::Mat &source);
    void generateSin(cv::Mat &dest, double shift = M_PI_4 / 4);
}

#endif