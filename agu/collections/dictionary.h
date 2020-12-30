#ifndef VSB_AGU_DICTIONARY_H
#define VSB_AGU_DICTIONARY_H

#include "priority_queue.h"
#include "../types/segment.h"

struct SegmentComparator {
    bool operator()(const Segment *a, const Segment *b) const {
        return a->cross.y < b->cross.y;
    }
};

class Dictionary {
private:
    std::set<Segment*, SegmentComparator> queue;

public:
    void insert(Segment *s);
    void erase(Segment *s);
    Segment* above(Segment *s);
    Segment* below(Segment *s);
    void swap(Segment *s1, Segment *s2, const cv::Point2f &p);
};

#endif //VSB_AGU_DICTIONARY_H