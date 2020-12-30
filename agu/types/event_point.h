#ifndef VSB_AGU_EPOINT_H
#define VSB_AGU_EPOINT_H

#include <ostream>
#include "segment.h"

enum EventType {
    Start = 0,
    End = 1,
    Intersect = 2
};

struct EventPoint {
public:
    cv::Point2f point;
    Segment *s1;
    Segment *s2;
    EventType type;

    EventPoint(const cv::Point2f &point, Segment *s1, EventType type) : point(point), s1(s1), s2(nullptr), type(type) {}
    EventPoint(const cv::Point2f &point, Segment *s1, Segment *s2) : point(point), s1(s1), s2(s2), type(EventType::Intersect) {}

    bool isLeft();
    bool isRight();

    bool operator==(const EventPoint &rhs) const;
    bool operator!=(const EventPoint &rhs) const;
    bool operator<(const EventPoint &rhs) const;
    bool operator>(const EventPoint &rhs) const;
    bool operator<=(const EventPoint &rhs) const;
    bool operator>=(const EventPoint &rhs) const;
    friend std::ostream &operator<<(std::ostream &os, const EventPoint &point);
};

#endif //VSB_AGU_EPOINT_H
