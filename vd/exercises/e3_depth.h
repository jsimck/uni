#ifndef VSB_VD_E3DEPTH_H
#define VSB_VD_E3DEPTH_H


#include <opencv2/core/mat.hpp>
#include "aexercise.h"
#include "../core/synthetic_scene.h"

class E3Depth : AExercise {
private:
    // Lookup tables
    static const int NORMAL_LUT_SIZE = 20;
    static const uchar NORMAL_LUT[NORMAL_LUT_SIZE][NORMAL_LUT_SIZE];

    std::vector<cv::Mat> depths;
    SyntheticScene synthScene;

    void loadImages(std::string path);
    void loadSyntheticScene(std::string path);

    cv::Vec3f xyz(float u, int halfWidth, float v, int halfHeight, float d);
public:
    void computeNormals(const cv::Mat &src, cv::Mat &dst, cv::Mat &quantized, int patchSize = 5);

    int run(int argc, char **argv) override;
};


#endif //VSB_VD_E3DEPTH_H
