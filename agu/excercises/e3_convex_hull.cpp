#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <cv.hpp>
#include "e3_convex_hull.h"
#include "../utils/generator.h"
#include "../utils/visualizer.h"
#include "../algorithms/convex_hull.h"

int E3ConvexHull::run(int argc, char **argv) {
    // Init scene and points
    cv::Mat scene;
    std::vector<cv::Point2f> points, hull;
    Generator::generatePoints(points, 100, 0.05f, 0.95f);

    // Calc convex hull
    ConvexHull convexHull;
    convexHull.divideAndConquer(points, hull);

    // Visualize
    Visualizer::initScene(scene, cv::Size(1280, 768));
    Visualizer::drawPoints(scene, points);
    Visualizer::drawPolygon(scene, hull);

    cv::imshow("Convex Hull", scene);
    cv::waitKey(0);

    return 0;
}
