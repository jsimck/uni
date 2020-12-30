#include "event_point.h"

bool EventPoint::isLeft() {
    return type == EventType::Start;
}

bool EventPoint::isRight() {
    return type == EventType::End;
}

bool EventPoint::operator==(const EventPoint &rhs) const {
    return point.x == rhs.point.x && point.y == rhs.point.y;
}

bool EventPoint::operator!=(const EventPoint &rhs) const {
    return !(rhs == *this);
}

bool EventPoint::operator<(const EventPoint &rhs) const {
    return point.x < rhs.point.x;
}

bool EventPoint::operator>(const EventPoint &rhs) const {
    return rhs < *this;
}

bool EventPoint::operator<=(const EventPoint &rhs) const {
    return !(rhs < *this);
}

bool EventPoint::operator>=(const EventPoint &rhs) const {
    return !(*this < rhs);
}

std::ostream &operator<<(std::ostream &os, const EventPoint &point) {
    if (point.type == EventType::Start) {
        os << "START: ";
    } else if (point.type == EventType::End) {
        os << "END: ";
    } else if (point.type == EventType::Intersect) {
        os << "INTERSECT: ";
    }

    os << "(" << point.point.x << ", " << point.point.y << ") "
       << "s1: ";

    if (point.s1 != nullptr) {
        os << *point.s1;
    } else {
        os << "false";
    }

    os << ", s2: ";

    if (point.s2 != nullptr) {
        os << *point.s2;
    } else {
        os << "false";
    }

    return os;
}