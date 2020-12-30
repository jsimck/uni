#include "e2_plane_sweeping.h"
#include "../algorithms/plane_sweeping.h"
#include "../utils/generator.h"

int E2PlaneSweeping::run(int argc, char **argv) {
    // Generate segments
    std::vector<Segment> segments;
//    Generator::generateUniqueSegments(segments, 100);

    // Add segments
    segments.emplace_back(Segment(0.31f, 0.65f, 0.7f, 0.07f));
    segments.emplace_back(Segment(0.47f, 0.16f, 0.94f, 0.91f));
    segments.emplace_back(Segment(0.27f, 0.59f, 0.55f, 0.78f));
    segments.emplace_back(Segment(0.08f, 0.39f, 0.39f, 0.26f));
    segments.emplace_back(Segment(0.04f, 0.26f, 0.34f, 0.52f));

    // Generate segments and find intersections
    PlaneSweeping::lineIntersection(segments);

    return 0;
}
