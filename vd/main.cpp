#include <iostream>
#include <opencv2/opencv.hpp>
#include <random>
#include "exercises/e1_cloud_scatter.h"
#include "exercises/e2_flow.h"
#include "exercises/e3_depth.h"

int main(int argc, char *argv[]) {
//    E1CloudScatter().run(argc, argv);
//    E2Flow().run(argc, argv);
    E3Depth().run(argc, argv);

    return 0;
}