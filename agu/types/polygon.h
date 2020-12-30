#ifndef VSB_AGU_POLYHON_H
#define VSB_AGU_POLYHON_H

#include <opencv2/core/types.hpp>
#include <utility>

struct Polygon {
private:
    float sign(cv::Point2f p1, cv::Point2f p2, cv::Point2f p3);
    bool pointInTriangle(cv::Point2f pt, cv::Point2f v1, cv::Point2f v2, cv::Point2f v3);
    bool testConvexBinary(const cv::Point2f &point, std::vector<cv::Point2f> &points, const cv::Point2f &center);
public:
    std::vector<cv::Point2f> points;

    explicit Polygon(std::vector<cv::Point2f> points = {}) : points(std::move(points)) {}

    cv::Point2f centerOfGravity();
    bool testConvex(const cv::Point2f &point);
    bool testConvexBinary(const cv::Point2f &point);
};

#endif //VSB_AGU_POLYHON_H
