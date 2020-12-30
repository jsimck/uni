#include <iostream>
#include "types/mat.h"
#include "algorithm/lu_open_mp.h"
#include "utils/timer.h"
#include "algorithm/lu_cpp_11.h"


int main() {
    while (true) {
        std::cout << "--------------------------------------------------------" << std::endl;
        std::cout << "                    LU FACTORIZATION                    " << std::endl;
        std::cout << "--------------------------------------------------------" << std::endl;

        ulong size = 0;
        int type = 0, verbose = 0, parallel = 0;

        std::cout << "Set matrix size (1000 recommended, 0 to quit program): " << std::endl;
        std::cin >> size;

        if (size <= 0) {
            break;
        }

        std::cout << "Parallel (0 - No, 1 - Yes): " << std::endl;
        std::cin >> parallel;

        if (parallel != 0) {
            std::cout << "Use OpenMP (0) / Use C++11 threads (1): " << std::endl;
            std::cin >> type;
        }

        std::cout << "Verbose (0 - No, 1 - Yes): " << std::endl;
        std::cin >> verbose;
        std::cout << "Computing... ";

        LU *lu;
        Mat matrix = Mat::generate(size, -100, 100), upper, lower, permutation;
        if (parallel == 0) {
            lu = new LUCpp11(1);
        } else if (type == 0) {
            lu = new LUOpenMP();
        } else {
            lu = new LUCpp11();
        }

        // Calculate
        Timer t;
        lu->calc(matrix, upper, lower, permutation);

        if (verbose) {
            std::cout << "Matrix:" << std::endl << matrix << std::endl;
            std::cout << "Upper:" << std::endl << upper << std::endl;
            std::cout << "Lower:" << std::endl << lower << std::endl;
            std::cout << "Permutation:" << std::endl << permutation << std::endl;
        }

        // Print results
        std::cout << "Took " << t.elapsed() << "s" << std::endl << std::endl;

        // Clear resources
        delete lu;
    }

    return 0;
}
