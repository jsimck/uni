#include <iostream>
#include "line.h"

cv::Point2f Line::intersect(const Line &line) {
    float x1 = start.x, x2 = end.x, x3 = line.start.x, x4 = line.end.x;
    float y1 = start.y, y2 = end.y, y3 = line.start.y, y4 = line.end.y;

    float d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    if (d == 0) return POINT_INVALID;

    // Get the x and y
    float pre = (x1 * y2 - y1 * x2), post = (x3 * y4 - y3 * x4);
    float x = (pre * (x3 - x4) - (x1 - x2) * post) / d;
    float y = (pre * (y3 - y4) - (y1 - y2) * post) / d;

    // Check if the x and y coordinates are within both lines
    if (x < std::min(x1, x2) || x > std::max(x1, x2) ||
        x < std::min(x3, x4) || x > std::max(x3, x4)) {
        return POINT_INVALID;
    }
    if (y < std::min(y1, y2) || y > std::max(y1, y2) ||
        y < std::min(y3, y4) || y > std::max(y3, y4)) {
        return POINT_INVALID;
    }

    return cv::Point2f(x, y);
}

bool Line::hasIntersect(const Line &line) {
    return this->intersect(line) != POINT_INVALID;
}

std::ostream &operator<<(std::ostream &os, const Line &line) {
    os << "start: (" << line.start.x << ", " << line.start.y << ") "
       << " end: ("  << line.end.x << ", " << line.end.y << ") ";

    return os;
}

cv::Vec2f Line::dir() {
    return cv::Vec2f(
        end.x - start.x,
        end.y - start.y
    );
}

float Line::angle(Line &line) {
    cv::Vec2f leDir = dir();
    cv::Vec2f l0Dir = line.dir();

    return cv::fastAtan2(l0Dir[0], l0Dir[1]) - cv::fastAtan2(leDir[0], leDir[1]);
}

cv::Vec3f Line::cross(Line &line) {
    cv::Vec2f leDir = dir();
    cv::Vec2f lxDir = line.dir();
    cv::Vec3f leDir3(leDir[0], leDir[1], 0), lxDir3(lxDir[0], lxDir[1], 0);

    return leDir3.cross(lxDir3);
}
