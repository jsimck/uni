#include "polygon.h"
#include "line.h"
#include "../utils/visualizer.h"

cv::Point2f Polygon::centerOfGravity() {
    cv::Point2f center(0, 0);
    size_t size = points.size();

    for (auto &p : points) {
        center += p;
    }

    center.x /= size;
    center.y /= size;

    return center;
}

float Polygon::sign(cv::Point2f p1, cv::Point2f p2, cv::Point2f p3) {
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool Polygon::pointInTriangle(cv::Point2f pt, cv::Point2f v1, cv::Point2f v2, cv::Point2f v3) {
    bool b1, b2, b3;

    b1 = sign(pt, v1, v2) < 0.0f;
    b2 = sign(pt, v2, v3) < 0.0f;
    b3 = sign(pt, v3, v1) < 0.0f;

    return ((b1 == b2) && (b2 == b3));
}

bool Polygon::testConvexBinary(const cv::Point2f &point, std::vector<cv::Point2f> &points, const cv::Point2f &center) {
    // If there are only 2 points, form triangle with center and check if point is inside triangle
    if (points.size() == 2) {
        return pointInTriangle(point, center, points[0], points[1]);
    }

    // Find center of gravity
    cv::Scalar cRed(0, 0, 255), cGreen(0, 255, 0);
    std::vector<cv::Point2f> nextPoints = points;

    // Init e and x vectors
    size_t mid = points.size() / 2;
    Line le(center, points[mid]);
    Line lx(center, point);

    // Find angle and cross product
//    float angle = le.angle(lx);
    cv::Vec3f cross = le.cross(lx);

    if (cross[2] < 0) { // Left
        nextPoints = std::vector<cv::Point2f>(points.begin(), points.begin() + mid + 1);
    } else { // Right
        nextPoints = std::vector<cv::Point2f>(points.begin() + mid, points.end());
    }

    return testConvexBinary(point, nextPoints, center);
}

bool Polygon::testConvex(const cv::Point2f &point) {
    Line pointLine(point, cv::Point2f(0, 0));
    int intersects = 0;
    size_t pSize = points.size();

    for (size_t i = 0; i < pSize - 1; i++) {
        Line l(points[i], points[i + 1]);
        intersects += pointLine.hasIntersect(l) ? 1 : 0;
    }

    // Check last Line from end to start
    Line l(points[pSize - 1], points[0]);
    intersects += pointLine.hasIntersect(l) ? 1 : 0;

    return intersects % 2 == 1 && intersects > 0;
}

bool Polygon::testConvexBinary(const cv::Point2f &point) {
    // Add start twice to the vector of points
    std::vector<cv::Point2f> nPoints = points;
    nPoints.emplace_back(points[0]);

    return testConvexBinary(point, nPoints, centerOfGravity());
}
