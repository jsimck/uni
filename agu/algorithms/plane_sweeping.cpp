#include <random>
#include "plane_sweeping.h"
#include "../collections/priority_queue.h"
#include "../types/event_point.h"
#include "../collections/dictionary.h"
#include "../utils/visualizer.h"

void PlaneSweeping::lineIntersection(std::vector<Segment> &segments) {
    Dictionary alpha;
    std::queue<EventPoint> delta;
    PriorityQueue<EventPoint> epsilon;

    // Draw results
    cv::Mat result, resultLine;
    Visualizer::initScene(result, cv::Size(1280, 768));
    for (auto &s : segments) {
        Visualizer::drawLine(result, s.line);
    }

    // Show results
    cv::imshow("PlaneSweeping", result);
    cv::waitKey(0);

    // Pre-processing, insert into self-ordering queue
    for (auto &s : segments) {
        epsilon.insert({s.line.start, &s, EventType::Start});
        epsilon.insert({s.line.end, &s, EventType::End});
    }

    // Plane sweeping alg
    do {
        EventPoint p = epsilon.pop();

        if (p.isLeft()) {
            Segment *s = p.s1;
            alpha.insert(s);
            Segment *s1 = alpha.above(s), *s2 = alpha.below(s);

            if (s1 != nullptr) {
                cv::Point2f iP = s->line.intersect(s1->line);
                if (iP != POINT_INVALID) {
                    delta.push(EventPoint(iP, s1, s));
                }
            }

            if (s2 != nullptr) {
                cv::Point2f iP = s->line.intersect(s2->line);
                if (iP != POINT_INVALID) {
                    delta.push(EventPoint(iP, s, s2));
                }
            }
        } else if (p.isRight()) {
            Segment *s = p.s1, *s1 = alpha.above(s), *s2 = alpha.below(s);

            if (s1 != nullptr && s2 != nullptr) {
                cv::Point2f iP = s1->line.intersect(s2->line);
                if (iP != POINT_INVALID) {
                    delta.push(EventPoint(iP, s1, s2));
                }
            }

            alpha.erase(s);
        } else {
            Segment *s1 = p.s1, *s2 = p.s2;
            Segment *s3 = alpha.above(s1), *s4 = alpha.below(s2);
            cv::Point2f iP;

            if (s3 != nullptr) {
                iP = s3->line.intersect(s2->line);
                if (iP != POINT_INVALID) {
                    delta.push(EventPoint(iP, s3, s2));
                }
            }

            if (s4 != nullptr) {
                iP = s1->line.intersect(s4->line);
                if (iP != POINT_INVALID) {
                    delta.push(EventPoint(iP, s1, s4));
                }
            }

            // Swap
            alpha.swap(s1, s2, iP);
        }

        // Print intersection
        while (!delta.empty()) {
            EventPoint i = delta.front();
            delta.pop();

            if (!epsilon.contains(i)) {
                epsilon.insert(i);
            }
        }

        Visualizer::drawPointWithCoordinates(result, p.point);
        resultLine = result.clone();
        Visualizer::drawVerticalLine(resultLine, p.point.x, cv::Scalar(255, 0, 0));
        cv::imshow("PlaneSweeping", resultLine);
        cv::waitKey(1);
    } while (!epsilon.isEmpty());

    // Show results
    cv::imshow("PlaneSweeping", resultLine);
    cv::waitKey(0);
}