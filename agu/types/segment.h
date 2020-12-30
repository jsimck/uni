#ifndef VSB_AGU_SEGMENT_H
#define VSB_AGU_SEGMENT_H

#include <opencv2/core/types.hpp>
#include <ostream>
#include "line.h"

struct Segment {
public:
    Line line;
    cv::Point2f cross;

    Segment(const cv::Point2f &start, const cv::Point2f &end) : line(start, end), cross(start) {}
    Segment(float x1, float y1, float x2, float y2) : line({x1, y1, x2, y2}), cross(cv::Point2f(x1, y1)) {}
    Segment(const cv::Point2f &start, const cv::Point2f &end, const cv::Point2f &cross) : line(start, end), cross(cross) {}
    Segment(float x1, float y1, float x2, float y2, const cv::Point2f &cross) : line({x1, y1, x2, y2}), cross(cross) {}

    bool operator<(const Segment &rhs) const;
    bool operator>(const Segment &rhs) const;
    bool operator<=(const Segment &rhs) const;
    bool operator>=(const Segment &rhs) const;
    friend std::ostream &operator<<(std::ostream &os, const Segment &segment);
};

#endif //VSB_AGU_SEGMENT_H