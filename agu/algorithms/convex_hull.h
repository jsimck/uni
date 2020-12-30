#ifndef VSB_AGU_CONVEX_HULL_H
#define VSB_AGU_CONVEX_HULL_H

#include <opencv2/core/types.hpp>

#define VISUALIZE

struct Point2fLessThan {
    bool operator() (const cv::Point2f& p1, const cv::Point2f& p2) {
        if (p1.x == p2.x) {
            return p1.y < p2.y;
        }

        return (p1.x < p2.x);
    }
};

class ConvexHull {
private:
    int orientation(const cv::Point2f &p1, const cv::Point2f &p2, const cv::Point2f &p3);

    void findMinMaxBoundaries(std::vector<cv::Point2f> &hull1, std::vector<cv::Point2f> &hull2, std::vector<size_t> &indices);
    void findTangentBoundaries(std::vector<cv::Point2f> &hull1, std::vector<cv::Point2f> &hull2, std::vector<size_t> &indices);

    void merge(std::vector<cv::Point2f> &hull1, std::vector<cv::Point2f> &hull2, std::vector<cv::Point2f> &result);
    void splitByX(std::vector<cv::Point2f> &points, std::vector<std::vector<cv::Point2f>> &result);
public:
    void divideAndConquer(std::vector<cv::Point2f> &points, std::vector<cv::Point2f> &hull);
};

#endif //VSB_AGU_CONVEX_HULL_H
