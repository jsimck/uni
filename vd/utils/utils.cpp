#include <cmath>
#include "utils.h"

float Utils::radf(float x) {
    return static_cast<float>(rad(x));
}

double Utils::rad(double x) {
    return (x) * (M_PI / 180.0);
}

float Utils::degf(float x) {
    return static_cast<float>(deg(x));
}

double Utils::deg(double x) {
    return (x) * (180.0 / M_PI);
}
