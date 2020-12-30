#include <iomanip>
#include <iostream>
#include <sstream>
#include "mat.h"

// Generator engines
std::random_device Mat::rd;
std::mt19937 Mat::gen = std::mt19937(rd());
std::uniform_real_distribution<float> Mat::dis = std::uniform_real_distribution<float>(0.0f, 1.0f);

Mat::Mat(ulong size, float val) {
    this->size = size;

    for (unsigned int i = 0; i < size; ++i) {
        std::vector<float> row;

        for (unsigned int j = 0; j < size; ++j) {
            row.emplace_back(val);
        }

        this->data.emplace_back(row);
    }
}

Mat Mat::generate(ulong size, float min, float max) {
    Mat mat(size);

    for (unsigned int i = 0; i < size; ++i) {
        std::vector<float> row;

        for (unsigned int j = 0; j < size; ++j) {
            float v = std::round((max - min) * dis(gen) + min);
            v = (v == -0) ? 0 : v;
            row.emplace_back(v);
        }

        mat.data.emplace_back(row);
    }

    return mat;
}

Mat Mat::generateUnitMatrix(ulong size) {
    Mat mat(size);

    for (unsigned int i = 0; i < size; ++i) {
        std::vector<float> row;

        for (unsigned int j = 0; j < size; ++j) {
            row.emplace_back(j == i ? 1 : 0);
        }

        mat.data.emplace_back(row);
    }

    return mat;
}

std::vector<float> &Mat::operator[](const unsigned int i) {
    return data[i];
}

std::string Mat::toWolframFormat() {
    std::ostringstream oss;
    bool firstRow = true;

    oss << "LU decomposition of {";
    for (auto &row : data) {
        bool firstCol = true;

        if (firstRow) {
            firstRow = false;
            oss << "{";
        } else {
            oss << ",{";
        }

        for (ulong j = 0; j < size; ++j) {
            if (firstCol) {
                firstCol = false;
                oss << row[j];
            } else {
                oss << "," << row[j];
            }
        }

        oss << "}";
    }
    oss <<"}";

    return oss.str();
}

std::ostream &operator<<(std::ostream &os, const Mat &mat) {
    for (auto &row : mat.data) {
        os << "|";
        for (auto &cell : row) {
            os << std::fixed << std::setprecision(2) << std::setw(8) << std::setfill(' ') << (cell == -0 ? 0 : cell);
        }
        os << "  |" << std::endl;
    }

    return os;
}
