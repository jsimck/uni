#ifndef VSB_ANO_CLASSIFIER_H
#define VSB_ANO_CLASSIFIER_H

#include <opencv2/opencv.hpp>
#include "structs/shape.h"
#include "structs/group.h"

namespace classifier {
    void threshold(cv::Mat &src, double threshold = 0.5);
    void indexObjects(cv::Mat &src, cv::Mat &dst, std::vector<Shape> &shapes);

    double moment(int x, int y, int p, int q);
    double momentNormalized(int x, int xt, int y, int yt, int p, int q);
    void momentMinMax(Shape &s);

    void calcFeatures(cv::Mat &index, std::vector<Shape> &shapes);
    void train(cv::Mat &src, cv::Mat &index, std::vector<Shape> &shapes);
    void recognize(std::vector<Group> &trainGroups, std::vector<Shape> &testShapes);

    void colorIndexedObjects(cv::Mat &src, cv::Mat &dst, std::vector<Shape> &shapes);
    void visualizeFeatures(cv::Mat &src, std::vector<Shape> &shapes, std::vector<Group> &groups, int size = 400);
    void annotate(cv::Mat &src, std::vector<Shape> shapes);

    // --- Clustering ---
    // Manual
    void generateClusters(std::vector<Group> &groups, std::vector<Shape> &shapes);

    // K-means
    int recalculateCenter(std::vector<Group> &centroids, std::vector<Shape> &shapes);
    void generateClustersKMeans(std::vector<Group> &centroids, std::vector<Shape> &shapes, int k = 3);

    // Neural network
    void neuralNetwork(std::vector<Shape> &trainShapes, std::vector<Shape> &testShapes);

    template<typename T>
    int maxIndex(T x, T y, T z) {
        if (x >= y && x >= z) {
            return 0;
        } else if (y >= x && y >= z) {
            return 1;
        } else if (z >= x && z >= y) {
            return 2;
        }
    }
}

#endif //VSB_ANO_CLASSIFIER_H
