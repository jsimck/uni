#ifndef VSB_ANO_SHAPE_H
#define VSB_ANO_SHAPE_H

#include <opencv2/opencv.hpp>
#include <ostream>

// Forward declaration
struct Group;

struct Shape {
public:
    int id;
    cv::Vec3b color;

    // Shape name (usually after match)
    std::string name;
    Group *group = nullptr;

    // Moments
    union {
        struct {
            double m00;
            double m01;
            double m10;

            double u00;
            double u11;
            double u20;
            double u02;
        };

        double data[7];
    } moments;

    // Features
    struct {
        cv::Point center;
        double perimeter;
        double area;
        double F1;
        double F2;
    } features;

    // Constructor
    Shape(int id, cv::Vec3b color) : id(id), color(color) {}
    ~Shape();

    // Operators
    friend std::ostream &operator<<(std::ostream &os, const Shape &shape);
    bool operator==(const Shape &shape) const;
    bool operator!=(const Shape &shape) const;
};

#endif //VSB_ANO_SHAPE_H
