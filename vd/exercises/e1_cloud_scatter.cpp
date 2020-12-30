#include "e1_cloud_scatter.h"
#include "../utils/utils.h"
#include <opencv2/opencv.hpp>
#include <random>

float gaussian(float x, float y, float A = 1.0f, float x0 = 0, float y0 = 0, float spreadX = 1.5f, float spreadY = 1.5f) {
    return A * std::exp(-(
        (Utils::sqr<float>(x - x0) / (2 * Utils::sqr<float>(spreadX))) +
        (Utils::sqr<float>(y - y0) / (2 * Utils::sqr<float>(spreadY)))
    ));
}

int E1CloudScatter::run(int argc, char **argv) {
    const int N = 300; // number of samples
    const float x0 = -3.0f; // sampling domain range
    const float y0 = -3.0f;
    const float x1 = 3.0f;
    const float y1 = 3.0f;
    const float radius = 1.0f;

    const int gd = 2; // dataset geometrical dimension
    cv::Mat sample_points(N, gd, CV_32F);
    cv::Mat sample_values(N, 1, CV_32F);

    // Init random
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> disX(x0, x1);
    std::uniform_real_distribution<float> disY(y0, y1);

    for (int i = 0; i < N; i++) {
        float x = disX(gen);
        float y = disY(gen);

        // Fill sample values and points
        sample_points.at<cv::Vec2f>(i) = cv::Vec2f(x, y);
        sample_values.at<float>(i) = gaussian(x, y);
    }

    // k-NN structures
    cv::flann::KDTreeIndexParams index;
    cv::flann::Index nn(sample_points, index);
    cv::flann::SearchParams search_params(256);

    // reconstruction
    cv::Mat query(1, gd, CV_32F);
    cv::Mat indices(N, 1, CV_32S);
    cv::Mat dists(N, 1, CV_32F);
    cv::Mat rec(800, 800, CV_32FC3), nearest(800, 800, CV_32FC3);

    for (int y = 0; y < rec.rows; y++) {
        for (int x = 0; x < rec.cols; x++) {
            // for each element of reconstructed domain do
            float xx = (x / (float) rec.cols) * (x1 - x0) + x0;
            float yy = (y / (float) rec.rows) * (y1 - y0) + y0;
            query.at<float>(0) = xx;
            query.at<float>(1) = yy;

            const int no_indices = nn.radiusSearch(query, indices, dists, radius, N, search_params);
            float W = 0, WP = 0;

            for (int i = 0; i < no_indices; ++i) {
                float v = sample_values.at<float>(indices.at<int>(i));
                float d = dists.at<float>(i);
                auto w = Utils::sqr<float>((static_cast<float>(std::fmax(0, radius - d))) / (radius * d));
                WP += w * v;
                W += w;
            }

            // Store results
            nearest.at<cv::Vec3f>(y, x) = cv::Vec3f(0, sample_values.at<float>(indices.at<int>(0)), 1.0f);
            rec.at<cv::Vec3f>(y, x) = cv::Vec3f(0, WP / W, 1.0f);
        }
    }

    cv::imshow("Flann", rec);
    cv::imshow("Nearest neighbour", nearest);
    cv::waitKey(0);

    return 0;
}
