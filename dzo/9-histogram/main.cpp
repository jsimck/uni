#include <stdio.h>
#include <opencv2/opencv.hpp>

#define SQR(x) ( ( x ) * ( x ) )
#define DEG2RAD(x) ( ( x ) * M_PI / 180.0 )
#define MY_MIN(X, Y) ( ( X ) < ( Y ) ? ( X ) : ( Y ) )
#define MY_MAX(X, Y) ( ( X ) > ( Y ) ? ( X ) : ( Y ) )
#define L 256

int cdfMin(cv::Mat &cdf) {
    int min = 1;

    for (int i = 0; i < cdf.rows; i++) {
        float newMin = cdf.at<float>(i);
        if (newMin < min) min = (int) newMin;
    }

    return min;
}

cv::Mat calcCdf(cv::Mat &hist) {
    // Calculate image integral
    cv::Mat cdfInt;
    cv::integral(hist, cdfInt, CV_32F);

    // Remove first column and row, since integral adds W + 1 and H + 1
    return cdfInt(cv::Rect(1, 1, 1, hist.rows - 1));
}

cv::Mat calcHistogram(cv::Mat &src) {
    cv::Mat hist = cv::Mat::zeros(L, 1, CV_32FC1);

    for (int y = 0; y < src.rows; y++) {
        for (int x = 0; x < src.cols; x++) {
            uchar &srcPx = src.at<uchar>(y, x);
            float &histPx = hist.at<float>(srcPx);
            histPx++;
        }
    }

    return hist;
}

cv::Mat drawHistogram(cv::Mat &hist) {
    cv::Mat histNorm;
    cv::normalize(hist, histNorm, 0.0f, 255.0f, CV_MINMAX);
    cv::Mat dst = cv::Mat::zeros(L, L, CV_8UC1);

    for (int x = 0; x < dst.cols; x++) {
        int vertical = cv::saturate_cast<int>(histNorm.at<float>(x));
        for (int y = L - 1; y > (L - vertical - 1); y--) {
            dst.at<uchar>(y, x) = 255;
        }
    }

    return dst;
}

int main() {
    cv::Mat src_8uc1_img, src_32fc1_hist, src_32fc1_cdf;
    src_8uc1_img = cv::imread("../data/uneq.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    cv::Mat dst_32fc1_normed = cv::Mat::zeros(src_8uc1_img.rows, src_8uc1_img.cols, CV_32FC1);

    // Get rows and cols
    int rows = src_8uc1_img.rows;
    int cols = src_8uc1_img.cols;
    int WxH = rows * cols;

    // Calc histogram and img integral (cdf)
    src_32fc1_hist = calcHistogram(src_8uc1_img);
    src_32fc1_cdf = calcCdf(src_32fc1_hist);

    // Get cdf min
    float min = cdfMin(src_32fc1_cdf);
    
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            int srcPx = cv::saturate_cast<int>(src_8uc1_img.at<uchar>(y, x));
            float cdf = src_32fc1_cdf.at<float>(srcPx);

            dst_32fc1_normed.at<float>(y, x) = roundf(((cdf - min) / (WxH - min)) * (L - 1));
        }
    }

    // Normalize result
    cv::normalize(dst_32fc1_normed, dst_32fc1_normed, 0.0f, 1.0f, CV_MINMAX);

    // Show results
    cv::imshow("Source", src_8uc1_img);
    cv::imshow("Result", dst_32fc1_normed);
    cv::imshow("Source Histogram", drawHistogram(src_32fc1_hist));
    cv::imshow("Result Histogram", drawHistogram(src_32fc1_cdf));

    cv::waitKey(0);
    return 0;
}
