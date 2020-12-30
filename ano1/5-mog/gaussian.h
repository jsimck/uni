#ifndef VSB_ANO_GAUSSIAN_H
#define VSB_ANO_GAUSSIAN_H

#include <opencv2/core/mat.hpp>
#include <vector>
#include <ostream>

struct Gaussian {
public:
    double sd;
    double u;
    double p;

    // Statics
    static void visualize(cv::Mat &dst, std::vector<std::vector<std::vector<Gaussian>>> &gaussians, cv::Point &center, int x);
    static double calcProbability(double X, Gaussian &g);
    static double sumProbability(double X, std::vector<Gaussian> &gaussians);
    static double conditionalProbability(double X, Gaussian &g, double sumP = 1.0);
    static int max(double X, std::vector<Gaussian> &gaussians);

    // Constructor
    Gaussian(double u = 0, double sd = 0, double p = 0) : sd(sd), u(u), p(p) {}

    // Methods
    void update(double X, double sumP, double alpha = 0.1, double minSD = 5);

    // Friends
    friend std::ostream &operator<<(std::ostream &os, const Gaussian &gaussian);
};

#endif //VSB_ANO_GAUSSIAN_H
