#include "segment.h"

bool Segment::operator<(const Segment &rhs) const {
    return this->cross.y < rhs.cross.y;
}

bool Segment::operator>(const Segment &rhs) const {
    return rhs < *this;
}

bool Segment::operator<=(const Segment &rhs) const {
    return !(rhs < *this);
}

bool Segment::operator>=(const Segment &rhs) const {
    return !(*this < rhs);
}

std::ostream &operator<<(std::ostream &os, const Segment &segment) {
    os << "(" << segment.line.start.x << ", " << segment.line.start.y << ", " << segment.line.end.x << ", " << segment.line.end.y << ")";
    return os;
}