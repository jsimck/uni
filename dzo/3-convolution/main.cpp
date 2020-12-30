#include <iostream>
#include <opencv2/opencv.hpp>
#include "convolution.h"

int FILTER_UNIFORM[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
int FILTER_GAUSSIAN[9] = {1, 2, 1, 2, -5, 2, 1, 2, 1};
int FILTER_SHARPEN[25] = {0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, -1, 5, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0,};
int FILTER_EDGE_ENHANCE[9] = {0, 0, 0, -1, 1, 0, 0, 0, 0};
int FILTER_EDGE_DETECT[9] = {0, 1, 0, 1, -4, 1, 0, 1, 0};
int FILTER_SOBEL_X[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};

int main() {
    cv::Mat src_8uc3_image;
    cv::Mat dest_8uc3_uniform;
    cv::Mat dest_8uc3_gauss;
    cv::Mat dest_8uc3_sharpen;
    cv::Mat dest_8uc3_edge_enhance;
    cv::Mat dest_8uc3_edge_detect;
    cv::Mat dest_8uc3_emboss;

    src_8uc3_image = cv::imread("../data/lena.png", CV_LOAD_IMAGE_COLOR);
    convolution<int, 3>(src_8uc3_image, dest_8uc3_uniform, FILTER_UNIFORM);
    convolution<int, 3>(src_8uc3_image, dest_8uc3_gauss, FILTER_GAUSSIAN);
    convolution<int, 5>(src_8uc3_image, dest_8uc3_sharpen, FILTER_SHARPEN);
    convolution<int, 3>(src_8uc3_image, dest_8uc3_edge_enhance, FILTER_EDGE_ENHANCE);
    convolution<int, 3>(src_8uc3_image, dest_8uc3_edge_detect, FILTER_EDGE_DETECT);
    convolution<int, 3>(src_8uc3_image, dest_8uc3_emboss, FILTER_SOBEL_X);

    cv::imshow("Original", src_8uc3_image);
    cv::imshow("FILTER_UNIFORM", dest_8uc3_uniform);
    cv::imshow("FILTER_GAUSSIAN", dest_8uc3_gauss);
    cv::imshow("FILTER_SHARPEN", dest_8uc3_sharpen);
    cv::imshow("FILTER_EDGE_ENHANCE", dest_8uc3_edge_enhance);
    cv::imshow("FILTER_EDGE_DETECT", dest_8uc3_edge_detect);
    cv::imshow("FILTER_SOBEL_X", dest_8uc3_emboss);

    // Contrast test using convertTo function and g(x, y) = alpha * f(x, y) + beta formula
    cv::Mat dest_8uc3_contrast;
    src_8uc3_image.convertTo(dest_8uc3_contrast, -1, 1.2f, 10);
    cv::imshow("Contrast", dest_8uc3_contrast);

    cv::waitKey(0);
    return 0;
}