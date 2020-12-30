#include <thread>
#include <random>
#include <iostream>
#include <mutex>
#include <functional>

void multithreadTest() {
    const size_t nThreads = std::thread::hardware_concurrency();
    const int N = 100000;
    size_t a[N], sum = 0;

    std::random_device seed;
    std::mt19937 gen(seed());
    std::uniform_int_distribution<ulong> dis(0, 10);

    for (int j = 0; j < N; ++j) {
        a[j] = dis(gen);
    }

    std::vector<std::thread> threads;
    std::mutex m;

    auto tStart = std::chrono::system_clock::now();
    for (size_t t = 0; t < nThreads; t++) {
        threads.emplace_back(std::bind(
            [&](const int start, const int end) {
                ulong _sum = 0;

                for (int i = start; i < end; i++) {
                    _sum += a[i];
                }

                // Protects shared data below from being simulatenously accessed
                // lock is unlocked when code reaches tEnd of scope
                std::lock_guard<std::mutex> guard(m);
                sum += _sum;
            }, t * (N / nThreads), (t + 1 == nThreads) ? N : (t + 1) * (N / nThreads)
        ));
    }

    // Wait till all threads finish
    for (auto &thread : threads) {
        thread.join();
    }

    auto tEnd = std::chrono::system_clock::now();
    std::chrono::duration<double> duration = tEnd - tStart;
    std::cout << "Sum: " << sum << " took: " << duration.count() << "s" << std::endl;
}