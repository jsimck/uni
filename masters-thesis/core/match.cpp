#include "match.h"

namespace tless {
    cv::Rect Match::scaledBB(const cv::Rect &rect, float scale, float newScale) {
        float multiplier = newScale / scale;

        return cv::Rect(
            static_cast<int>(rect.x * multiplier),
            static_cast<int>(rect.y * multiplier),
            static_cast<int>(rect.width * multiplier),
            static_cast<int>(rect.height * multiplier)
        );
    }

    float Match::overlap(const Match &m) {
        return (this->normObjBB & m.normObjBB).area() / static_cast<float>(std::min(this->normObjBB.area(), m.normObjBB.area()));
    }

    bool Match::operator<(const Match &rhs) const {
        return score < rhs.score;
    }

    bool Match::operator>(const Match &rhs) const {
        return rhs < *this;
    }

    bool Match::operator<=(const Match &rhs) const {
        return !(rhs < *this);
    }

    bool Match::operator>=(const Match &rhs) const {
        return !(*this < rhs);
    }

    std::ostream &operator<<(std::ostream &os, const Match &match) {
        os << "id: " << match.t->id
           << " scale: " << match.scale
           << " t: " << match.t
           << " score: " << match.score;

        return os;
    }

    cv::Point Match::objBBCenter() {
        return {objBB.x + (objBB.width / 2), objBB.y + (objBB.height / 2)};
    }

    float Match::jaccard(const cv::Rect &r1) const {
        return (objBB & r1).area() / static_cast<float>((objBB | r1).area());
    }
}