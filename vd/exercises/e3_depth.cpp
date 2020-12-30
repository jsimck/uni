#include <sstream>
#include <iomanip>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "e3_depth.h"

const uchar E3Depth::NORMAL_LUT[E3Depth::NORMAL_LUT_SIZE][E3Depth::NORMAL_LUT_SIZE] = {
        {32, 32, 32, 32, 32, 32, 64, 64, 64, 64, 64, 64,  64,  64,  64,  128, 128, 128, 128, 128},
        {32, 32, 32, 32, 32, 32, 32, 64, 64, 64, 64, 64,  64,  64,  128, 128, 128, 128, 128, 128},
        {32, 32, 32, 32, 32, 32, 32, 64, 64, 64, 64, 64,  64,  64,  128, 128, 128, 128, 128, 128},
        {32, 32, 32, 32, 32, 32, 32, 32, 64, 64, 64, 64,  64,  128, 128, 128, 128, 128, 128, 128},
        {32, 32, 32, 32, 32, 32, 32, 32, 64, 64, 64, 64,  64,  128, 128, 128, 128, 128, 128, 128},
        {32, 32, 32, 32, 32, 32, 32, 32, 64, 64, 64, 64,  64,  128, 128, 128, 128, 128, 128, 128},
        {16, 32, 32, 32, 32, 32, 32, 32, 32, 64, 64, 64,  128, 128, 128, 128, 128, 128, 128, 128},
        {16, 16, 16, 32, 32, 32, 32, 32, 32, 64, 64, 64,  128, 128, 128, 128, 128, 128, 1,   1},
        {16, 16, 16, 16, 16, 16, 32, 32, 32, 32, 64, 128, 128, 128, 128, 1,   1,   1,   1,   1},
        {16, 16, 16, 16, 16, 16, 16, 16, 32, 32, 64, 128, 128, 1,   1,   1,   1,   1,   1,   1},
        {16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 1,  1,   1,   1,   1,   1,   1,   1,   1,   1},
        {16, 16, 16, 16, 16, 16, 16, 16, 8,  8,  4,  2,   2,   1,   1,   1,   1,   1,   1,   1},
        {16, 16, 16, 16, 16, 16, 8,  8,  8,  8,  4,  2,   2,   2,   2,   1,   1,   1,   1,   1},
        {16, 16, 16, 8,  8,  8,  8,  8,  8,  4,  4,  4,   2,   2,   2,   2,   2,   2,   1,   1},
        {16, 8,  8,  8,  8,  8,  8,  8,  8,  4,  4,  4,   2,   2,   2,   2,   2,   2,   2,   2},
        {8,  8,  8,  8,  8,  8,  8,  8,  4,  4,  4,  4,   4,   2,   2,   2,   2,   2,   2,   2},
        {8,  8,  8,  8,  8,  8,  8,  8,  4,  4,  4,  4,   4,   2,   2,   2,   2,   2,   2,   2},
        {8,  8,  8,  8,  8,  8,  8,  8,  4,  4,  4,  4,   4,   2,   2,   2,   2,   2,   2,   2},
        {8,  8,  8,  8,  8,  8,  8,  4,  4,  4,  4,  4,   4,   4,   2,   2,   2,   2,   2,   2},
        {8,  8,  8,  8,  8,  8,  8,  4,  4,  4,  4,  4,   4,   4,   2,   2,   2,   2,   2,   2}
};

int E3Depth::run(int argc, char **argv) {
    // Load depth images
//    loadImages("depth/");
    cv::Mat normals, quantized;

//    for (auto &depth : depths) {
//        cv::Mat normals;
//        computeNormals(depth, normals, quantized, 5);
//        cv::imshow("normals", normals);
//        cv::imshow("quantized", quantized);
//        cv::waitKey(1);
//    }

    // Load synthetic
    loadSyntheticScene("synthetic_depth/scene_eda_2.yml.gz");
    computeNormals(synthScene.depth, normals, quantized, 3);
    cv::imshow("normals", normals);
    cv::imshow("org", synthScene.normals);
    cv::imshow("quantized", quantized);
    cv::waitKey(0);

    return 0;
}

void E3Depth::loadImages(std::string path) {
    std::string baseName = "depth_";
    std::ostringstream oss;
    std::cout << "Loading depth images...";

    for (int i = 0; i < 150; i++) {
        oss.str("");
        oss << path << baseName;
        oss << std::setw(6) << std::setfill('0') << i;
        oss << ".yml.gz";

        // Read file
        cv::Mat depth;
        cv::FileStorage fs(oss.str(), cv::FileStorage::READ);
        assert(fs.isOpened());

        // Load depth image
        fs["depth"] >> depth;
        depth.convertTo(depth, CV_32FC1); // convert to floats
        depths.push_back(depth);
        fs.release();
    }

    std::cout << "DONE! 150 images loaded." << std::endl;
}

void E3Depth::loadSyntheticScene(std::string path) {
    std::cout << "Loading depth images...";
    cv::FileStorage fs(path, cv::FileStorage::READ);
    assert(fs.isOpened());

    // Load depth and params
    fs["camera_width"] >> synthScene.camWidth;
    fs["camera_height"] >> synthScene.camHeight;
    fs["fov_y"] >> synthScene.fov;
    fs["pixel_size"] >> synthScene.pixelSize;

    fs["view_from"] >> synthScene.viewFrom;
    fs["view_at"] >> synthScene.viewAt;
    fs["view_matrix"] >> synthScene.view;
    fs["view_t_matrix"] >> synthScene.viewT;

    fs["depth"] >> synthScene.depth;
    fs["normals"] >> synthScene.normals;
    fs.release();

    std::cout << "DONE! Synthetic image loaded." << std::endl;
}

cv::Vec3f E3Depth::xyz(float u, const int halfWidth, float v, const int halfHeight, float d) {
    static const float f = 570.342f;

    return cv::Vec3f(((u - halfWidth) * d) / f, -((-v + halfHeight) * d) / f, d);
}

void E3Depth::computeNormals(const cv::Mat &src, cv::Mat &dst, cv::Mat &quantized, int patchSize) {
    assert(src.type() == CV_32F);
    dst = cv::Mat::zeros(src.size(), CV_32FC3);
    quantized = cv::Mat::zeros(src.size(), CV_8UC1);

    const int offset = patchSize / 2;
    const int halfHeight = src.rows / 2;
    const int halfWidth = src.cols / 2;
    const int maxDiff = 10;

    for (int y = offset; y < src.rows - offset; y++) {
        for (int x = offset; x < src.cols - offset; x++) {
            cv::Mat cov = cv::Mat::zeros(3, 3, CV_32FC1);
            float Ex = 0, Ey = 0, Ez = 0;
            int validCount = patchSize * patchSize;
            float dAvg = src.at<float>(y, x);

            // Find mean values
            for (int yy = y - offset; yy <= y + offset; yy++) {
                for (int xx = x - offset; xx <= x + offset; xx++) {
                    // Get xyz vector in screen space
                    float d = src.at<float>(yy, xx);

                    // Skip wrong pixels
                    if (d <= 0 || std::abs(dAvg - d) > maxDiff) {
                        --validCount;
                        continue;
                    }

                    // Get vector in screen space
                    cv::Vec3f v = xyz(xx, halfWidth, yy, halfHeight, d);

                    // Store mean value
                    Ex += v[0];
                    Ey += v[1];
                    Ez += v[2];
                }
            }

            // Check if there are any valid pixels
            if (validCount <= 0) {
                continue;
            }

            // Normalize mean values
            const float normInv = 1.0f / validCount;
            Ex *= normInv;
            Ey *= normInv;
            Ez *= normInv;

            // Compute covariance matrix
            for (int yy = y - offset; yy <= y + offset; yy++) {
                for (int xx = x - offset; xx <= x + offset; xx++) {
                    // Get xyz vector in screen space
                    float d = src.at<float>(yy, xx);

                    // Skip wrong pixels
                    if (d <= 0 || std::abs(dAvg - d) > maxDiff) continue;

                    // Get vector in screen space
                    cv::Vec3f v = xyz(xx, halfWidth, yy, halfHeight, d);

                    // Calculate upper part of cov matrix
                    cov.at<float>(0, 0) += (v[0] - Ex) * (v[0] - Ex);
                    cov.at<float>(0, 1) += (v[0] - Ex) * (v[1] - Ey);
                    cov.at<float>(0, 2) += (v[0] - Ex) * (v[2] - Ez);

                    cov.at<float>(1, 1) += (v[1] - Ey) * (v[1] - Ey);
                    cov.at<float>(1, 2) += (v[1] - Ey) * (v[2] - Ez);

                    cov.at<float>(2, 2) += (v[2] - Ez) * (v[2] - Ez);
                }
            }

            // Copy to bottom half of matrix
            cov.at<float>(1, 0) = cov.at<float>(0, 1);
            cov.at<float>(2, 0) = cov.at<float>(0, 2);
            cov.at<float>(2, 1) = cov.at<float>(1, 2);

            // Normalize matrix
            cov *= normInv;

            // Calculate eigen vector
            cv::Mat eigenValues, eigenVectors;
            cv::eigen(cov, eigenValues, eigenVectors);

            // Normal is last row, where the variance is the lowest
            cv::Vec3f n = eigenVectors.row(2);
            n = cv::normalize(n);

            if (n.dot(cv::Vec3f(0, 0, 1)) < 0) {
                n = -n;
            }

            dst.at<cv::Vec3f>(y, x) = n;

            // Quantize normal
            auto vX = static_cast<int>(n[0] * (NORMAL_LUT_SIZE / 2) + (NORMAL_LUT_SIZE / 2));
            auto vY = static_cast<int>(n[1] * (NORMAL_LUT_SIZE / 2) + (NORMAL_LUT_SIZE / 2));
            quantized.at<uchar>(y, x) = NORMAL_LUT[vY][vX];
        }
    }

    cv::medianBlur(quantized, quantized, 5);
}