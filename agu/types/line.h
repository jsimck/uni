#ifndef VSB_AGU_LINE_H
#define VSB_AGU_LINE_H

#include <opencv2/core/types.hpp>
#include <ostream>

#define POINT_INVALID cv::Point2f(-1.0f, -1.0f)

struct Line {
    cv::Point2f start;
    cv::Point2f end;

    Line(const cv::Point2f &start, const cv::Point2f &end) : start(start), end(end) {}
    Line(float x1, float y1, float x2, float y2) : start(cv::Point2f(x1, y1)), end(cv::Point2f(x2, y2)) {}

    cv::Vec2f dir();
    cv::Vec3f cross(Line &line);
    float angle(Line &line);

    cv::Point2f intersect(const Line &line);
    bool hasIntersect(const Line &line);

    friend std::ostream &operator<<(std::ostream &os, const Line &line);
};

#endif //VSB_AGU_LINE_H
