#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <omp.h>
#include "multiplication.h"
#include "mat.h"
#include "timer.h"

int main() {
    Mat a = Mat::random(1500, 1500), b = Mat::random(1500, 1500), result = Mat::zeros(1500, 1500);
    Timer t;
    mult::multiplyRVC(a.data, b.data, result.data);
    std::cout << t << std::endl;


    std::ofstream ofs("results.csv");
    assert(ofs.is_open());
    ofs << "n;" << "t;" << "rcv;" << "rvc;" << "crv;" << "cvr;" << "vcr;" << "vrc;"
            << "t;" << "rcv;" << "rvc;" << "crv;" << "cvr;" << "vcr;" << "vrc;"
            << "t;" << "rcv;" << "rvc;" << "crv;" << "cvr;" << "vcr;" << "vrc;"
            << "t;" << "rcv;" << "rvc;" << "crv;" << "cvr;" << "vcr;" << "vrc;" << std::endl;

    for (uint i = 500; i <= 1550; i += 150) {
        ofs << i << ";";
        std::cout << "Computing: " << i << "... " << std::endl;

        for (uint j = 1; j <= 8; j *= 2) {
            Timer t;
            Mat a = Mat::random(i, i), b = Mat::random(i, i), result = Mat::zeros(i, i);
            ofs << j << ";";

            // Start testing 2D Matrix
            t.reset();
            omp_set_num_threads(j);
            mult::multiplyRCV(a, b, result);
            ofs << t << ";";
            result.reset();

            t.reset();
            omp_set_num_threads(j);
            mult::multiplyRVC(a, b, result);
            ofs << t << ";";
            result.reset();

            t.reset();
            omp_set_num_threads(j);
            mult::multiplyCRV(a, b, result);
            ofs << t << ";";
            result.reset();

            t.reset();
            omp_set_num_threads(j);
            mult::multiplyCVR(a, b, result);
            ofs << t << ";";
            result.reset();

            t.reset();
            omp_set_num_threads(j);
            mult::multiplyVCR(a, b, result);
            ofs << t << ";";
            result.reset();

            t.reset();
            omp_set_num_threads(j);
            mult::multiplyVRC(a, b, result);
            ofs << t << ";";
            result.reset();
        }

        std::cout << "DONE!" << std::endl;
        ofs << std::endl;
    }

    ofs.close();
    return 0;
}