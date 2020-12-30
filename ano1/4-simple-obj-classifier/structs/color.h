#ifndef VSB_ANO_COLORS_H
#define VSB_ANO_COLORS_H

#include <opencv2/core/types.hpp>

class Color {
private:
    static cv::Vec3b colors[12];
    static int counter;

    Color() {}
public:
    static cv::Vec3b red;
    static cv::Vec3b pink;
    static cv::Vec3b purple;
    static cv::Vec3b deeppurple;
    static cv::Vec3b indigo;
    static cv::Vec3b blue;
    static cv::Vec3b cyan;
    static cv::Vec3b teal;
    static cv::Vec3b green;
    static cv::Vec3b yellow;
    static cv::Vec3b brown;
    static cv::Vec3b lightgreen;

    static cv::Vec3b randomColor();
};

#endif //VSB_ANO_COLORS_H