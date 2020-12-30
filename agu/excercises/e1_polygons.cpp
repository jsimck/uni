#include "e1_polygons.h"
#include <opencv2/opencv.hpp>
#include "../utils/visualizer.h"
#include "../utils/generator.h"

int E1Polygons::run(int argc, char **argv) {
    // Init scene
    cv::Mat result, resultBinary;
    Visualizer::initScene(result, cv::Size(1280, 768));
    auto poly = Generator::generatePolygon(20);

    // Generate points
    const int N = 300;
    std::vector<cv::Point2f> points, pointsBinary;
    Generator::generatePoints(points, N);
    Generator::generatePoints(pointsBinary, N);

    // Draw
    Visualizer::drawPolygon(result, poly);
    resultBinary = result.clone();
    cv::Scalar colorRed(0, 0, 255), colorBlue(255, 0, 0);

    // Run point localization
    for (int i = 0; i < N; i++) {
        Visualizer::drawPoint(result, points[i], poly.testConvex(points[i]) ? colorBlue : colorRed);
        Visualizer::drawPoint(resultBinary, pointsBinary[i], poly.testConvexBinary(pointsBinary[i]) ? colorBlue : colorRed);
    }

    // Show results
    cv::imshow("Polygon", result);
    cv::imshow("Polygon - binary", resultBinary);
    cv::waitKey(0);

    return 0;
}
