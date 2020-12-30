#ifndef VSB_VD_E2FLOW_H
#define VSB_VD_E2FLOW_H

#include "aexercise.h"

class E2Flow : AExercise {
private:
    float dt;
    int scale, iterations, numPoints;
    std::vector<cv::Mat> flows;
    std::vector<cv::Point2f> points;
    cv::Mat lines, result;

    void loadData();
    cv::Point2f getSubPixel(const cv::Mat &flow, cv::Point2f pt);
    cv::Point2f rungeKutta(const cv::Mat &flow, cv::Point2f p, float dt);
    std::vector<cv::Point2f> movePoints(const cv::Mat &flow, std::vector<cv::Point2f> &points, float dt);

    void computeCurl(const cv::Mat &flow, cv::Mat &curl, cv::Mat &color);
    void visualizeFlow(const cv::Mat &flow, cv::Mat &scene);
public:
    explicit E2Flow(int scale = 7, float dt = 0.01f, int iterations = 50, int numPoints = 20)
            : scale(scale), dt(dt), iterations(iterations), numPoints(numPoints) {}

    int run(int argc, char **argv) override;
};

#endif //VSB_VD_E2FLOW_H
