#include "visualizer.h"

void Visualizer::drawPolygon(cv::Mat &scene, std::vector<cv::Point2f> &points, const cv::Scalar &color, int weight) {
    assert(scene.type() == CV_8UC3);
    assert(!scene.empty());
    const size_t pSize = points.size();

    for (size_t i = 0; i < pSize - 1; i++) {
        cv::Point2f p1(scene.cols * points[i].x, scene.rows * points[i].y);
        cv::Point2f p2(scene.cols * points[i + 1].x, scene.rows * points[i + 1].y);
        cv::line(scene, p1, p2, color, weight, CV_AA);
    }

    cv::Point2f p1(scene.cols * points[pSize - 1].x, scene.rows * points[pSize - 1].y);
    cv::Point2f p2(scene.cols * points[0].x, scene.rows * points[0].y);
    cv::line(scene, p1, p2, color, weight, CV_AA);
}

void Visualizer::drawPolygon(cv::Mat &scene, Polygon &poly, const cv::Scalar &color, int weight) {
    drawPolygon(scene, poly.points, color, weight);
}

void Visualizer::drawLine(const cv::Mat &scene, const cv::Point2f &p1, const cv::Point2f &p2, const cv::Scalar &color, int weight) {
    drawLine(scene, Line(p1, p2), color);
}

void Visualizer::drawLine(const cv::Mat &scene, const Line &line, const cv::Scalar &color, int weight) {
    assert(scene.type() == CV_8UC3);
    assert(!scene.empty());

    cv::Point2f p1(scene.cols * line.start.x, scene.rows * line.start.y);
    cv::Point2f p2(scene.cols * line.end.x, scene.rows * line.end.y);
    cv::line(scene, p1, p2, color, weight, CV_AA);
}

void Visualizer::drawArrowedLine(const cv::Mat &scene, const cv::Point2f &p1, const cv::Point2f &p2, const cv::Scalar &color, int weight) {
    drawArrowedLine(scene, Line(p1, p2), color);
}

void Visualizer::drawArrowedLine(const cv::Mat &scene, const Line &line, const cv::Scalar &color, int weight) {
    assert(scene.type() == CV_8UC3);
    assert(!scene.empty());

    cv::Point2f p1(scene.cols * line.start.x, scene.rows * line.start.y);
    cv::Point2f p2(scene.cols * line.end.x, scene.rows * line.end.y);
    cv::arrowedLine(scene, p1, p2, color, weight, CV_AA, 0, 0.05);
}

void Visualizer::drawVerticalLine(const cv::Mat &scene, float x, const cv::Scalar &color, int weight) {
    assert(scene.type() == CV_8UC3);
    assert(!scene.empty());

    cv::line(scene, cv::Point2f(x * scene.cols, 0), cv::Point2f(x * scene.cols, scene.rows), color, weight, CV_AA);
}

void Visualizer::drawHorizontalLine(const cv::Mat &scene, float y, const cv::Scalar &color, int weight) {
    assert(scene.type() == CV_8UC3);
    assert(!scene.empty());

    cv::line(scene, cv::Point2f(0, y * scene.rows), cv::Point2f(scene.cols, y * scene.rows), color, weight, CV_AA);
}

void Visualizer::drawPoint(const cv::Mat &scene, const cv::Point2f &point, const cv::Scalar &color) {
    assert(scene.type() == CV_8UC3);
    assert(!scene.empty());

    cv::circle(scene, cv::Point2f(point.x * scene.cols, point.y * scene.rows), 2, color, -1, CV_AA);
}

void Visualizer::drawPoints(const cv::Mat &scene, std::vector<cv::Point2f> &points, const cv::Scalar &color) {
    assert(!points.empty());
    for (auto &p : points) {
        drawPoint(scene, p, color);
    }
}

void Visualizer::drawPointWithCoordinates(const cv::Mat &scene, const cv::Point2f &point, const cv::Scalar &color) {
    assert(scene.type() == CV_8UC3);
    assert(!scene.empty());

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << "(" << point.x << ", " << point.y << ")";

    drawPoint(scene, point, color);
    setLabel(scene, oss.str(), cv::Point2f(point.x * scene.cols, point.y * scene.rows) + cv::Point2f(10, 5), 0.35);
}

void Visualizer::drawPointsWithCoordinates(const cv::Mat &scene, std::vector<cv::Point2f> &points, const cv::Scalar &color) {
    assert(!points.empty());
    for (auto &p : points) {
        drawPointWithCoordinates(scene, p, color);
    }
}

void Visualizer::initScene(cv::Mat &scene, const cv::Size &canvasSize, const cv::Scalar &color) {
    scene = cv::Mat(canvasSize, CV_8UC3, cv::Scalar::all(255));
}

void Visualizer::setLabel(const cv::Mat &scene, const std::string &label, const cv::Point2f &origin, double scale,
                          const cv::Scalar &fColor, const cv::Scalar &bColor, int thickness, int padding, int fontFace) {
    assert(scene.type() == CV_8UC3);
    assert(!scene.empty());

    cv::Size text = cv::getTextSize(label, fontFace, scale, thickness, nullptr);
    rectangle(scene, origin + cv::Point2f(-padding - 1, padding + 2),
              origin + cv::Point2f(text.width + padding, -text.height - padding - 2), bColor, CV_FILLED);
    putText(scene, label, origin, fontFace, scale, fColor, thickness, CV_AA);
}

void Visualizer::drawPointsWithLabel(const cv::Mat &scene, std::vector<cv::Point2f> &points, const std::string &label, const cv::Scalar &color) {
    drawPoints(scene, points, color);
    for (auto &point : points) {
        setLabel(scene, label, cv::Point2f(point.x * scene.cols, point.y * scene.rows) + cv::Point2f(10, 5), 0.35);
    }
}

void Visualizer::drawPointWithLabel(const cv::Mat &scene, const cv::Point2f &point, const std::string &label, const cv::Scalar &color) {
    drawPoint(scene, point, color);
    setLabel(scene, label, cv::Point2f(point.x * scene.cols, point.y * scene.rows) + cv::Point2f(10, 5), 0.35);
}
