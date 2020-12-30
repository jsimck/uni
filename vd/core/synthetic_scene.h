#ifndef VSB_VD_SYNTHETICSCENE_H
#define VSB_VD_SYNTHETICSCENE_H

#include <opencv2/core/mat.hpp>

struct SyntheticScene {
public:
    int camWidth, camHeight;
    float fov, pixelSize;
    cv::Mat viewAt, viewFrom, view, viewT;
    cv::Mat depth, normals;

    SyntheticScene() {}
};

#endif //VSB_VD_SYNTHETICSCENE_H
