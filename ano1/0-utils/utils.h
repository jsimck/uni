#ifndef ANO1_UTILS_H
#define ANO1_UTILS_H

#include <string>

#define SQR(x) ((x) * (x))
#define RAD(x) ((x) * (M_PI / 180.0))
#define DEG(x) ((x) * (180.0 / M_PI))

#define SAFE_DELETE(p) { \
    if ( p != NULL ) \
    { \
        delete p; \
        p = NULL; \
    } \
}

#define SAFE_DELETE_ARRAY(p) { \
    if ( p != NULL ) \
    { \
        delete [] p; \
        p = NULL; \
    } \
}

// ---------------------------------------
// | Mat type |  C1 |  C2  |  C3  |  C4  |
// ---------------------------------------
// | CV_8U    |  0  |  8   |  16  |  24  |
// | CV_8S    |  1  |  9   |  17  |  25  |
// | CV_16U   |  2  |  10  |  18  |  26  |
// | CV_16S   |  3  |  11  |  19  |  27  |
// | CV_32S   |  4  |  12  |  20  |  28  |
// | CV_32F   |  5  |  13  |  21  |  29  |
// | CV_64F   |  6  |  14  |  22  |  30  |
// ---------------------------------------
namespace utils {
    std::string matType2Str(int type) {
        std::string r;

        uchar depth = type & CV_MAT_DEPTH_MASK;
        uchar chans = 1 + (type >> CV_CN_SHIFT);

        switch (depth) {
            case CV_8U:  r = "8U"; break;
            case CV_8S:  r = "8S"; break;
            case CV_16U: r = "16U"; break;
            case CV_16S: r = "16S"; break;
            case CV_32S: r = "32S"; break;
            case CV_32F: r = "32F"; break;
            case CV_64F: r = "64F"; break;
            default:     r = "User"; break;
        }

        r += "C";
        r += (chans + '0');

        return r;
    }
}

#endif