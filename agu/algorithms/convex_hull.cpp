#include <opencv2/core/mat.hpp>
#include "convex_hull.h"
#include "../utils/generator.h"
#include "../utils/visualizer.h"

void ConvexHull::divideAndConquer(std::vector<cv::Point2f> &points, std::vector<cv::Point2f> &hull) {
#ifdef VISUALIZE
    cv::Mat scene, sceneHull, sceneOneHull;
    Visualizer::initScene(scene, cv::Size(1280, 768));
    Visualizer::drawPoints(scene, points, cv::Scalar(0 ,0, 255));
    sceneHull = scene.clone();
    sceneOneHull = scene.clone();
#endif

    std::vector<std::vector<cv::Point2f>> hulls, tmpHulls;
    std::sort(points.begin(), points.end(), Point2fLessThan());

    // find small polygons
    splitByX(points, hulls);
    int j = 0;

    // Try merging
    while (hulls.size() != 1) {
        tmpHulls.clear();
        size_t hullsSize = hulls.size();

#ifdef VISUALIZE
        sceneHull = scene.clone();
#endif

        for (size_t i = 0; i < hullsSize - 1; i += 2) {
            std::vector<cv::Point2f> tmpHull;
            merge(hulls[i], hulls[i + 1], tmpHull);
            tmpHulls.emplace_back(tmpHull);

#ifdef VISUALIZE
            sceneOneHull = sceneHull.clone();
            Visualizer::drawPolygon(sceneOneHull, hulls[i]);
            Visualizer::drawPolygon(sceneOneHull, hulls[i + 1]);
            cv::imshow("Convex Hull", sceneOneHull);
            cv::waitKey(1);
            Visualizer::drawPolygon(sceneHull, tmpHull);
            cv::imshow("Convex Hull", sceneHull);
            cv::waitKey(1);
#endif
        }

        if (hullsSize % 2 == 1) {
            tmpHulls.emplace_back(hulls[hullsSize - 1]);
        }

        // Push to parent hulls and run again
        hulls.swap(tmpHulls);
        j = (j + 1) % 4;
    }

    hull.swap(hulls[0]);
}

void ConvexHull::splitByX(std::vector<cv::Point2f> &points, std::vector<std::vector<cv::Point2f>> &result) {
    size_t pSize = points.size();

    if (pSize > 3) {
        size_t mid = pSize / 2;
        std::vector<cv::Point2f> lPoints, rPoints;

        lPoints = std::vector<cv::Point2f>(points.begin(), points.begin() + mid);
        rPoints = std::vector<cv::Point2f>(points.begin() + mid, points.end());

        splitByX(lPoints, result);
        splitByX(rPoints, result);
    } else {
        // Reorganize points to make sure they're in clockwise orientation
        if (pSize == 3) {
            std::vector<cv::Point2f> sPoints;
            cv::Point2f p1 = points[0];
            cv::Point2f p2 = points[1];
            cv::Point2f p3 = points[2];

            // Push first point
            sPoints.emplace_back(p1);

            // Push the one on top first
            if (p2.y < p3.y) {
                sPoints.emplace_back(p2);
                sPoints.emplace_back(p3);
            } else {
                sPoints.emplace_back(p3);
                sPoints.emplace_back(p2);
            }

            assert(sPoints.size() == 3);
            result.emplace_back(sPoints);
        } else {
            assert(points.size() == 2);
            result.emplace_back(points);
        }
    }
}

void ConvexHull::merge(std::vector<cv::Point2f> &hull1, std::vector<cv::Point2f> &hull2, std::vector<cv::Point2f> &result) {
    // Find top and bottom boundaries of each hull
    std::vector<size_t> indices; // p1Ti[0], p1Bi[1], p2Ti[2], p2Bi[3]
    findTangentBoundaries(hull1, hull2, indices);
    size_t hull1S = hull1.size();
    size_t hull2S = hull2.size();

    // Put new points
    for (size_t i = indices[1]; i < hull1S; i = (i + 1) % hull1S) {
        result.emplace_back(hull1[i]);
        if (i == indices[0]) { break; }
    }

    for (size_t i = indices[2]; i < hull2S; i = (i + 1) % hull2S) {
        result.emplace_back(hull2[i]);
        if (i == indices[3]) { break; }
    }
}

void ConvexHull::findMinMaxBoundaries(std::vector<cv::Point2f> &hull1, std::vector<cv::Point2f> &hull2,
                                      std::vector<size_t> &indices) {
    // Find top and bottom boundaries
    cv::Point2f p1T(0, 1), p1B(0, 0), p2T(0, 1), p2B(0, 0);
    size_t p1Ti = 0, p1Bi = 0, p2Ti = 0, p2Bi = 0;
    size_t hull1S = hull1.size();
    size_t hull2S = hull2.size();

    // Find top and bottom of both of those
    for (size_t i = 0; i < hull1S; i++) {
        if (hull1[i].y < p1T.y) {
            p1T = hull1[i];
            p1Ti = i;
        }

        if (hull1[i].y >= p1B.y) {
            p1B = hull1[i];
            p1Bi = i;
        }
    }

    for (size_t i = 0; i < hull2S; i++) {
        if (hull2[i].y < p2T.y) {
            p2T = hull2[i];
            p2Ti = i;
        }

        if (hull2[i].y >= p2B.y) {
            p2B = hull2[i];
            p2Bi = i;
        }
    }

    // Save results (we only need indexes)
    indices.emplace_back(p1Ti);
    indices.emplace_back(p1Bi);
    indices.emplace_back(p2Ti);
    indices.emplace_back(p2Bi);
}

void ConvexHull::findTangentBoundaries(std::vector<cv::Point2f> &hull1, std::vector<cv::Point2f> &hull2, std::vector<size_t> &indices) {
    // Visualize
    cv::Mat scene, sceneTmp;
    Visualizer::initScene(scene, cv::Size(1280, 768));
    Visualizer::drawPolygon(scene, hull1);
    Visualizer::drawPolygon(scene, hull2);

    // Find leftmost and rightmost point
    cv::Point2f rH1(0, 0), lH2(1, 1);
    size_t iH1 = 0, iH2;
    size_t hull1S = hull1.size();
    size_t hull2S = hull2.size();

    // Find Right of left
    for (size_t i = 0; i < hull2S; i++) {
        if (hull2[i].x < lH2.x) {
            lH2 = hull2[i];
            iH2 = i;
        }
    }

    // Find leftmost of right
    for (size_t i = 0; i < hull1S; i++) {
        if (hull1[i].x > rH1.x) {
            rH1 = hull1[i];
            iH1 = i;
        }
    }

    // Find upper tangent
    size_t iH1T = iH1, iH2T = iH2;
    bool intersect = true;

    while (intersect) {
        intersect = false;

        // Check lines [b, a] [a, (prev p)], if [a, (prev p)] line is above [b, a], move a to prev point [clockwise]
        while (orientation(hull2[iH2T], hull1[iH1T], hull1[(hull1S + iH1T - 1) % hull1S]) >= 0) {
            intersect = true;
            iH1T = (hull1S + iH1T - 1) % hull1S;
        }

        // Check lines [a, b] [b, (next p)], if [b, (next p)] line is above [a, b], move b to next point [clockwise]
        while (orientation(hull1[iH1T], hull2[iH2T], hull2[(iH2T + 1) % hull2S]) < 0) {
            intersect = true;
            iH2T = (iH2T + 1) % hull2S;
        }
    }

    // Find lower tangent
    size_t iH1B = iH1, iH2B = iH2;
    intersect = true;

    while (intersect) {
        intersect = false;

        // Check lines [b, a] [a, (next p)], if [a, (next p)] line is below [b, a], move a to next point [clockwise]
        while (orientation(hull2[iH2B], hull1[iH1B], hull1[(iH1B + 1) % hull1S]) < 0) {
            intersect = true;
            iH1B = (iH1B + 1) % hull1S;
        }

        // Check lines [a, b] [b, (prev p)], if [b, (prev p)] line is above [b, a], move b to prev point [clockwise]
        while (orientation(hull1[iH1B], hull2[iH2B], hull2[(hull2S + iH2B - 1) % hull2S]) >= 0) {
            intersect = true;
            iH2B = (hull2S + iH2B - 1) % hull2S;
        }
    }

    // Save results
    indices.emplace_back(iH1T);
    indices.emplace_back(iH1B);
    indices.emplace_back(iH2T);
    indices.emplace_back(iH2B);
}

int ConvexHull::orientation(const cv::Point2f &p1, const cv::Point2f &p2, const cv::Point2f &p3) {
    float res = (p2.y - p1.y) * (p3.x - p2.x) - (p3.y - p2.y) * (p2.x - p1.x);

    if (res == 0) { return 0; }
    if (res > 0) { return -1; }
    return 1;
}
