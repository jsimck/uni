#ifndef VSB_AGU_PLANESWEEPING_H
#define VSB_AGU_PLANESWEEPING_H

#include <opencv2/opencv.hpp>
#include <vector>
#include "../types/segment.h"

class PlaneSweeping {
public:
    static void lineIntersection(std::vector<Segment> &segments);
};

#endif //VSB_AGU_PLANESWEEPING_H
