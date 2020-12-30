#include "color.h"

int Color::counter = 0;
cv::Vec3b Color::colors[12] = {
    cv::Vec3b(244, 67, 54),
    cv::Vec3b(233, 30, 99),
    cv::Vec3b(156, 39, 176),
    cv::Vec3b(103, 58, 183),
    cv::Vec3b(63, 81, 181),
    cv::Vec3b(33, 150, 243),
    cv::Vec3b(0, 188, 212),
    cv::Vec3b(0, 150, 136),
    cv::Vec3b(76, 175, 80),
    cv::Vec3b(255, 235, 59),
    cv::Vec3b(121, 85, 72),
    cv::Vec3b(118, 255, 3)
};

cv::Vec3b Color::red = colors[0];
cv::Vec3b Color::pink = colors[1];
cv::Vec3b Color::purple = colors[2];
cv::Vec3b Color::deeppurple = colors[3];
cv::Vec3b Color::indigo = colors[4];
cv::Vec3b Color::blue = colors[5];
cv::Vec3b Color::cyan = colors[6];
cv::Vec3b Color::teal = colors[7];
cv::Vec3b Color::green = colors[8];
cv::Vec3b Color::yellow = colors[9];
cv::Vec3b Color::brown = colors[10];
cv::Vec3b Color::lightgreen = colors[11];

cv::Vec3b Color::randomColor() {
    counter = (counter + 1) % 12;
    return colors[counter];
}
