#include "dft.h"

void dft::generateSin(cv::Mat &dest, double shift) {
    for (int y = 0; y < dest.rows; y++) {
        for (int x = 0; x < dest.cols; x++) {
            double sinX = sin(x * shift);
            dest.at<double>(y, x) = sinX > 0 ? sinX : 0;
        }
    }
}

void dft::flipQuadrants(cv::Mat &source) {
    // Get half of image rows and cols
    int hRows = source.rows / 2;
    int hCols = source.cols / 2;

    for (int y = 0; y < hRows; y++) {
        for (int x = 0; x < hCols; x++) {
            // Extract px from each quadrant
            double tl = source.at<double>(y, x); // top left px
            double tr = source.at<double>(y, x + hCols); // top right px
            double bl = source.at<double>(y + hRows, x); // bottom left px
            double br = source.at<double>(y + hRows, x + hCols); // bottom right px

            // Compose image
            source.at<double>(y, x) = br;
            source.at<double>(y, x + hCols) = bl;
            source.at<double>(y + hRows, x) = tr;
            source.at<double>(y + hRows, x + hCols) = tl;
        }
    }
}

void dft::dft(cv::Mat &source, cv::Mat &furier) {
    cv::Mat normalized;

    // Helper vars
    int M = source.cols;
    int N = source.cols;
    double sqrtMN_d = 1.0 / sqrt(M * N);
    double PIPI = M_PI * 2;
    double sumR = 0, sumI = 0;
    double M_d = 1.0 / M, N_d = 1.0 / N;

    // Convert and normalize
    source.convertTo(normalized, CV_64FC1, 1.0 / 255.0 * sqrtMN_d);
    furier = cv::Mat(M, N, CV_64FC2);

    for (int k = 0; k < M; k++) {
        for (int l = 0; l < N; l++) {
            sumR = sumI = 0;

            for (int m = 0; m < M; m++) {
                for (int n = 0; n < N; n++) {
                    double px = normalized.at<double>(m, n);
                    double expX = -PIPI * (m * k * M_d + n * l * N_d);

                    sumR += px * cos(expX);
                    sumI += px * sin(expX);
                }
            }

            furier.at<cv::Vec2d>(k, l) = cv::Vec2d(sumR, sumI);
        }
    }
}

void dft::dftPhase(cv::Mat &furier, cv::Mat &dest) {
    int M = furier.rows;
    int N = furier.cols;

    // Create result matrix
    dest = cv::Mat(M, N, CV_64FC1);

    for (int m = 0; m < M; m++) {
        for (int n = 0; n < N; n++) {
            cv::Vec2d F = furier.at<cv::Vec2d>(m, n);
            dest.at<double>(m, n) = atan(F[1] / F[0]);
        }
    }
}

void dft::dftPower(cv::Mat &furier, cv::Mat &dest) {
    int M = furier.rows;
    int N = furier.cols;

    // Create result matrix
    dest = cv::Mat(M, N, CV_64FC1);

    for (int m = 0; m < M; m++) {
        for (int n = 0; n < N; n++) {
            cv::Vec2d F = furier.at<cv::Vec2d>(m, n);
            double amp = sqrt(SQR(F[0]) + SQR(F[1]));
            dest.at<double>(m, n) = log(SQR(amp));
        }
    }

    // Normalize and flip quadrants
    cv::normalize(dest, dest, 0.0, 1.0, CV_MINMAX);
    flipQuadrants(dest);
}

void dft::idft(cv::Mat &furier, cv::Mat &dest) {
    // Helper vars
    int M = furier.cols;
    int N = furier.cols;
    double sqrtMN_d = 1.0 / sqrt(M * N);
    double PIPI = M_PI * 2;
    double sum = 0;
    double M_d = 1.0 / M, N_d = 1.0 / N;

    // Create dest matrix
    dest = cv::Mat(M, N, CV_64FC1);

    for (int m = 0; m < M; m++) {
        for (int n = 0; n < N; n++) {
            sum = 0;

            for (int k = 0; k < M; k++) {
                for (int l = 0; l < N; l++) {
                    double expX = PIPI * ((k * m * M_d) + (l * n * N_d));

                    // Compute real and imaginary parts
                    double realX = cos(expX) * sqrtMN_d;
                    double imagX = sin(expX) * sqrtMN_d;

                    // Get furier transform
                    cv::Vec2d F = furier.at<cv::Vec2d>(k, l);

                    // Sum
                    sum += F[0] * realX - F[1] * imagX;
                }
            }

            dest.at<double>(m, n) = sum;
        }
    }

    cv::normalize(dest, dest, 0.0, 1.0, CV_MINMAX);
}

cv::Mat dft::circleMask(int rows, int cols, int radius, float foreground, float background) {
    cv::Mat result = cv::Mat::ones(rows, cols, CV_32FC1) * background;
    cv::circle(result, cv::Point(rows / 2, cols / 2), radius, foreground, -1);

    return result;
}