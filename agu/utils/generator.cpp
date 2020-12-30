#include <cassert>
#include <cv.hpp>
#include <iostream>
#include "generator.h"
#include "utils.h"

// Generator engines
std::random_device Generator::rd;
//std::mt19937 Generator::gen = std::mt19937(rd());
std::mt19937 Generator::gen = std::mt19937(5);
std::uniform_real_distribution<float> Generator::dis = std::uniform_real_distribution<float>(0.0f, 1.0f);

void Generator::generatePoints(std::vector<cv::Point2f> &points, size_t count, float min , float max) {
    assert(count > 0);
    points.reserve(count);

    // Generate points
    for (size_t i = 0; i < count; i++) {
        float x = min + (dis(gen) * (max - min));
        float y = min + (dis(gen) * (max - min));
        points.emplace_back(cv::Point2f(x, y));
    }
}

void Generator::generateUniqueSegments(std::vector<Segment> &segments, size_t count) {
    assert(count > 0);
    segments.reserve(count);

    // Generate segments
    for (size_t i = 0; i < count; i++) {
        float x1, y1, x2, y2;

        do {
            x1 = dis(gen);
            y1 = dis(gen);
            x2 = dis(gen);
            y2 = dis(gen);
        } while (x1 == x2 || y1 == y2 || x2 < x1);

        segments.emplace_back(Segment(x1, y1, x2, y2, cv::Point2f(x1, y1)));
    }
}

Polygon Generator::generatePolygon(size_t count) {
    assert(count > 0);

    // Draw points around circle
    Polygon p;
    cv::Point2f center(0.5f, 0.5f);
    float radius = 0.25f, step = 360.0f / count;

    for (float angle = 0; angle < 360.0f; angle += step) {
        p.points.emplace_back(cv::Point2f(
                static_cast<float>(center.x + radius * std::cos(Utils::rad(angle))),
                static_cast<float>(center.y + radius * std::sin(Utils::rad(angle)))
        ));
    }

    return p;
}
