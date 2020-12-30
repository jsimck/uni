#ifndef VSB_AGU_VISUALIZER_H
#define VSB_AGU_VISUALIZER_H

#include <opencv2/opencv.hpp>
#include "../types/polygon.h"
#include "../types/line.h"

class Visualizer {
public:
    // Draw elements
    static void drawPolygon(cv::Mat &scene, std::vector<cv::Point2f> &points, const cv::Scalar &color = cv::Scalar::all(0), int weight = 1);
    static void drawPolygon(cv::Mat &scene, Polygon &poly, const cv::Scalar &color = cv::Scalar::all(0), int weight = 1);

    static void drawLine(const cv::Mat &scene, const Line &line, const cv::Scalar &color = cv::Scalar::all(0), int weight = 1);
    static void drawLine(const cv::Mat &scene, const cv::Point2f &p1, const cv::Point2f &p2, const cv::Scalar &color = cv::Scalar::all(0), int weight = 1);
    static void drawArrowedLine(const cv::Mat &scene, const Line &line, const cv::Scalar &color = cv::Scalar::all(0), int weight = 1);
    static void drawArrowedLine(const cv::Mat &scene, const cv::Point2f &p1, const cv::Point2f &p2, const cv::Scalar &color = cv::Scalar::all(0), int weight = 1);
    static void drawVerticalLine(const cv::Mat &scene, float x, const cv::Scalar &color = cv::Scalar::all(0), int weight = 1);
    static void drawHorizontalLine(const cv::Mat &scene, float y, const cv::Scalar &color = cv::Scalar::all(0), int weight = 1);


    static void drawPoint(const cv::Mat &scene, const cv::Point2f &point, const cv::Scalar &color = cv::Scalar(0, 0, 255));
    static void drawPoints(const cv::Mat &scene, std::vector<cv::Point2f> &points, const cv::Scalar &color = cv::Scalar(0, 0, 255));
    static void drawPointWithCoordinates(const cv::Mat &scene, const cv::Point2f &point, const cv::Scalar &color = cv::Scalar(0, 0, 255));
    static void drawPointsWithCoordinates(const cv::Mat &scene, std::vector<cv::Point2f> &points, const cv::Scalar &color = cv::Scalar(0, 0, 255));
    static void drawPointWithLabel(const cv::Mat &scene, const cv::Point2f &point, const std::string &label, const cv::Scalar &color = cv::Scalar(0, 0, 255));
    static void drawPointsWithLabel(const cv::Mat &scene, std::vector<cv::Point2f> &points, const std::string &label, const cv::Scalar &color = cv::Scalar(0, 0, 255));

    // Helpers
    static void initScene(cv::Mat &scene, const cv::Size &canvasSize = cv::Size(1280, 768), const cv::Scalar &color = cv::Scalar::all(255));
    static void setLabel(const cv::Mat &scene, const std::string &label, const cv::Point2f &origin, double scale = 0.4, const cv::Scalar &fColor = cv::Scalar::all(255),
                         const cv::Scalar &bColor = cv::Scalar::all(0), int thickness = 1, int padding = 1, int fontFace = CV_FONT_HERSHEY_SIMPLEX);
};

#endif //VSB_AGU_VISUALIZER_H
