#ifndef FRACTAL_RANGE_H
#define FRACTAL_RANGE_H

#include <cwchar>

template<typename T>
struct Range {
public:
    T min;
    T max;

    Range(T min, T max) : min(min), max(max) {}
};

#endif //FRACTAL_RANGE_H
