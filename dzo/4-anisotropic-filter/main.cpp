#include <iostream>
#include <opencv2/opencv.hpp>

#define SQR(x) (x * x)

double g(double I, double sigma) {
    return exp(-(SQR(I) / SQR(sigma)));
}

int main() {
    cv::Mat src_8uc1;
    cv::Mat result_64fc1;
    cv::Mat result_64fc1_t;

    src_8uc1 = cv::imread("../data/lena.png", CV_LOAD_IMAGE_GRAYSCALE);
    src_8uc1.convertTo(result_64fc1, CV_64FC1, 1.0/255.0);
    result_64fc1.copyTo(result_64fc1_t);
    cv::imshow("Original", src_8uc1);

    double lambda = 0.1;
    double sigma = 0.015;

    for (int i = 0; i < 500; i++) {
        for (int y = 1; y < result_64fc1.rows - 1; y++) {
            for (int x = 1; x < result_64fc1.cols - 1; x++) {
                double I = result_64fc1.at<double>(y, x);

                double iN = result_64fc1.at<double>(y - 1, x);
                double iS = result_64fc1.at<double>(y + 1, x);
                double iE = result_64fc1.at<double>(y, x + 1);
                double iW = result_64fc1.at<double>(y, x - 1);

                double dN = iN - I;
                double dS = iS - I;
                double dE = iE - I;
                double dW = iW - I;

                double cN = g(dN, sigma);
                double cS = g(dS, sigma);
                double cE = g(dE, sigma);
                double cW = g(dW, sigma);

                double It = I + lambda * (cN * dN + cS * dS + cE * dE + cW * dW);

                result_64fc1_t.at<double>(y, x) = It;
            }
        }

        result_64fc1 = result_64fc1_t.clone();
        cv::putText(result_64fc1_t, std::to_string(i), cv::Point(10, 30), CV_FONT_HERSHEY_SCRIPT_SIMPLEX, 1.0, cv::Scalar(1.0), 2);
        cv::imshow("Anisotropic", result_64fc1_t);
        cv::waitKey(1);
    }

    cv::waitKey(0);
    return 0;
}