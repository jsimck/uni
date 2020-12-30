#ifndef DZO_5_DFT_H
#define DZO_5_DFT_H

#include <opencv2/opencv.hpp>

#define SQR(x) x * x

namespace dft {
    void dft(cv::Mat &source, cv::Mat &furier);
    void idft(cv::Mat &furier, cv::Mat &dest);

    void dftPhase(cv::Mat &furier, cv::Mat &dest);
    void dftPower(cv::Mat &furier, cv::Mat &dest);

    void frequencyFilter(cv::Mat &source, cv::Mat &mask, cv::Mat &dest);
    void generateSin(cv::Mat &dest, double shift = M_PI_4 / 4);

    // Flips 1 <-> 3 and 2 <-> 4 quadrants
    template<typename T>
    void flipQuadrants(cv::Mat &source) {
        // Get half of image rows and cols
        int hRows = source.rows / 2;
        int hCols = source.cols / 2;

        for (int y = 0; y < hRows; y++) {
            for (int x = 0; x < hCols; x++) {
                // Extract px from each quadrant
                T tl = source.at<T>(y, x); // top left px
                T tr = source.at<T>(y, x + hCols); // top right px
                T bl = source.at<T>(y + hRows, x); // bottom left px
                T br = source.at<T>(y + hRows, x + hCols); // bottom right px

                // Compose image
                source.at<T>(y, x) = br;
                source.at<T>(y, x + hCols) = bl;
                source.at<T>(y + hRows, x) = tr;
                source.at<T>(y + hRows, x + hCols) = tl;
            }
        }
    }

    template<typename T>
    void dftFilter(cv::Mat &furier, cv::Mat &mask) {
        // Flip quadrants for better computation
        dft::flipQuadrants<cv::Vec2d>(furier);

        for (int y = 0; y < furier.rows; y++) {
            for (int x = 0; x < furier.cols; x++) {
                cv::Vec2d &fPx = furier.at<cv::Vec2d>(y, x);
                T mPx = mask.at<T>(y, x);

                // Reset furier frequencies
                if (mPx == 0) {
                    fPx[0] = 0;
                    fPx[1] = 0;
                }
            }
        }

        // Flip quadrants back to original values
        dft::flipQuadrants<cv::Vec2d>(furier);
    }
}

#endif