#include "timer.h"

double Timer::elapsed() const {
    return std::chrono::duration_cast<second>(clock::now() - beginning).count();
}

void Timer::reset() {
    beginning = clock::now();
}

std::ostream &operator<<(std::ostream &os, const Timer &timer) {
    os << timer.elapsed();
    return os;
}
