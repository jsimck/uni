#include "dictionary.h"

void Dictionary::insert(Segment *s) {
    queue.insert(s);
}

void Dictionary::erase(Segment *s) {
    auto sf = queue.find(s);

    if (sf != queue.end()) {
        queue.erase(sf);
    }
}

Segment *Dictionary::above(Segment *s) {
    auto upper = queue.upper_bound(s);

    if (upper != queue.end()) {
        return *upper;
    }

    return nullptr;
}

Segment *Dictionary::below(Segment *s) {
    auto lower = queue.lower_bound(s);

    if (lower != queue.begin()) {
        return *(--lower);
    }

    return nullptr;
}

void Dictionary::swap(Segment *s1, Segment *s2, const cv::Point2f &p) {
    this->erase(s1);
    this->erase(s2);

//    std::swap<cv::Point2f>(s1->cross, s2->cross);

    this->insert(s1);
    this->insert(s2);
}
