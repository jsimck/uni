#ifndef VSB_AGU_GENERATOR_H
#define VSB_AGU_GENERATOR_H

#include <opencv2/core/types.hpp>
#include <random>
#include "../types/segment.h"
#include "../types/polygon.h"

class Generator {
private:
    static std::random_device rd;
    static std::mt19937 gen;
    static std::uniform_real_distribution<float> dis;
public:
    static void generatePoints(std::vector<cv::Point2f> &points, size_t count = 100, float min = 0.0f, float max = 1.0f);
    static void generateUniqueSegments(std::vector<Segment> &segments, size_t count = 100);
    static Polygon generatePolygon(size_t count = 100);
};

#endif //VSB_AGU_GENERATOR_H
