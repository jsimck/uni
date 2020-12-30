#include <iomanip>
#include "mat.h"

std::random_device Mat::seed;
std::mt19937 Mat::rnd(Mat::seed());

Mat::Mat(uint rows, uint cols) : rows(rows), cols(cols) {
    data = new unsigned long*[rows];

    for (uint y = 0; y < rows; y++) {
        data[y] = new unsigned long[cols];
    }
}

Mat::~Mat() {
    for (uint y = 0; y < rows; y++) {
        delete[] data[y];
    }

    delete[] data;
}

Mat Mat::zeros(uint rows, uint cols) {
    Mat mat(rows, cols);

    for (uint y = 0; y < rows; y++) {
        for (uint x = 0; x < cols; x++) {
            mat[y][x] = 0;
        }
    }

    return mat;
}

Mat Mat::random(uint rows, uint cols) {
    static std::uniform_int_distribution<ulong> dis(1, 10);
    Mat mat(rows, cols);

    for (uint y = 0; y < rows; y++) {
        for (uint x = 0; x < cols; x++) {
            mat[y][x] = dis(rnd);
        }
    }

    return mat;
}

void Mat::reset() {
    for (uint y = 0; y < rows; y++) {
        for (uint x = 0; x < cols; x++) {
            data[y][x] = 0;
        }
    }
}


std::ostream &operator<<(std::ostream &os, const Mat &mat) {
    for (uint y = 0; y < mat.rows; y++) {
        for (uint x = 0; x < mat.cols; x++) {
            os << std::setw(4) << std::setfill(' ') << mat.data[y][x];
        }
        os << std::endl;
    }

    return os;
}

ulong* Mat::operator[](uint row) {
    return data[row];
}