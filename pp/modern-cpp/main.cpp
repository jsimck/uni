#include <iostream>
#include <random>
#include <thread>
#include <functional>
#include <mutex>
#include "test.h"

#define LINE_BREAK "------------------------------------"

std::weak_ptr<Test> wp;
void observe() {
    std::cout << "use_count == " << wp.use_count() << ": ";
    if (auto spt = wp.lock()) {
        std::cout << *spt << std::endl;
    }
    else {
        std::cout << "wp is expired" << std::endl;
    }
}

template<typename T, typename U>
auto multiply(T x, U y) -> decltype(x * y) {
    return x * y;
}

constexpr long double operator"" _deg(long double deg) {
    return deg * 3.141592 / 180.0;
}

static void usability() {
    // Automatic type deduction
    auto vali = 5 * 10;
    auto x = multiply(5, 5.0);
    auto y = multiply(5.0f, 5.0f);
    auto z = multiply(5, 5);

    std::cout << "multiply(5, 5.0) -> " << x << typeid(x).name() << std::endl;
    std::cout << "multiply(5.0f, 5.0f) -> " << y << typeid(y).name() << std::endl;
    std::cout << "multiply(5, 5) -> " << z << typeid(z).name() << std::endl;

    // Move semantic
    std::cout << LINE_BREAK << std::endl;
    std::string str = "Hello cpp!";
    std::cout << str << " [" << str.length() << "]" << std::endl;

    std::string strMoved = std::move(str);
    std::cout << str << " [" << str.length() << "]" << std::endl;
    std::cout << strMoved << " [" << strMoved.length() << "]" << std::endl;

    // User defined literals
    std::cout << LINE_BREAK << std::endl;
    auto rad = 180.0_deg;
    std::cout << "rad: " << rad << std::endl;

    // Copy and move constructors
    std::cout << LINE_BREAK << std::endl;
    Test t1;
    std::cout << "t1: " << t1 << std::endl;
    Test t2 = t1;
    std::cout << "t1: " << t1 << " | t2: " << t2 << std::endl;
    Test t3 = std::move(t1);
    std::cout << "t1: " << t1 << " | t2: " << t2 << " | t3: " << t3 << std::endl;

    // Tuples
    std::cout << LINE_BREAK << std::endl;
    auto student  = std::make_tuple("SIM0178", "Jan", "Simecek");
    std::cout << std::get<0>(student) << " - " << std::get<1>(student) << " " << std::get<2>(student) << std::endl;
}

void functions() {
    // Std bind & lambdas
    // []        no access to external variables
    // [x, &y]   x is passed by value, y by reference
    // [&]       all variables passed by reference
    // [=]       all variables passed by value
    // [&, x]    x passed by value, rest by reference
    auto fn = [/* external variables */](float x, float y) -> float {
        return x / y;
    };
    std::cout << "fn(5, 10) -> " << fn(5, 10) << std::endl;

    auto fn1 = std::bind(fn, 1, std::placeholders::_1); // forwarding call wrapper
    std::cout << "fn1(10) -> " << fn1(10) << std::endl;

    auto fn2 = std::bind(fn, std::placeholders::_2, std::placeholders::_1);
    std::cout << "fn2(5, 10) -> " << fn2(5, 10) << std::endl;

    std::function<float(float, float)> fn3 = fn; // function wrapper
    std::cout << "fn3(5, 10) -> " << fn3(5, 10) << std::endl;
}

void smartPointers() {
    // Shared pointers
    auto sp1 = std::make_shared<Test>();
    std::cout << "use count of sp1 == " << sp1.use_count() << std::endl;

    // Increase reference count in child scope
    {
        auto sp2 = sp1;
        auto sp3 = sp1;
        auto sp4 = sp1;
        std::cout << "use count of sp1 == " << sp1.use_count() << std::endl; // Pointer is now reference in 4 objects
    }
    std::cout << "use count of sp1 == " << sp1.use_count() << std::endl; // Referenced counter decreased, objects are out of scope
    std::cout << LINE_BREAK << std::endl;

    // Unique pointers
    std::unique_ptr<Test> up1(new Test());
    Test obj = *up1; // Copies object up1 points to
    std::cout << "up1: " << *up1 << " | obj: " << obj << std::endl;
    std::cout << LINE_BREAK << std::endl;

    // Weak pointers, pointer that holds non-owning "weak" reference to an object
    // that is managed by shared_ptr. It must be converted to shared ptr, in order to use it.
    {
        auto wsp = std::make_shared<Test>();
        wp = wsp;

        observe();
    }

    observe();
}


void multithread() {
    using ulong = unsigned long;
    const ulong N = 1000;
    ulong a[N], sum = 0;

    std::random_device seed;
    std::mt19937 rnd(seed());
    std::uniform_int_distribution<unsigned long> dis(0, 10);

    for (int i = 0; i < N; i++) {
        a[i] = dis(rnd);
    }

    std::mutex m;
    const ulong nThreads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;

    for (int i = 0; i < nThreads; i++) {
        threads.emplace_back(
            std::bind([&](const int start, const int end) {
                ulong _sum = 0;
                for (int j = start; j < end; j++) {
                    _sum += a[j];
                }

                std::lock_guard<std::mutex> guard(m);
                sum += _sum;
            }, i * (N / nThreads), (i + 1) == nThreads ? N : (i + 1) * (N / nThreads))
        );
    }

    for (int i = 0; i < nThreads; i++) {
        threads[i].join();
    }

    std::cout << sum << std::endl;
}

int main() {
//    usability();
//    std::cout << std::endl << std::endl;
//
//    functions();
//    std::cout << std::endl << std::endl;
//
//    smartPointers();
//    std::cout << std::endl << std::endl;

    // Multithreading
    multithread();
}