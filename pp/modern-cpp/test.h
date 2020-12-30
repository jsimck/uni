#ifndef MODERN_CPP_TEST_H
#define MODERN_CPP_TEST_H

#include <vector>
#include <iostream>

class Test {
private:
    int x = 10; // Default values in classes and structs
    std::vector<int> y{0, 1, 2, 3, 4, 5}; // List initializers
public:
    Test() : Test{10} {} // Delegating constructor
    Test(int v) : x(v) {
        std::cout << "Test was constructed!" << std::endl;
    }

    ~Test() {
        std::cout << "Test was destroyed!" << std::endl;
    }

    Test(const Test &rhs) { // copy resources
        x = rhs.x;
        y = rhs.y;
        std::cout << "Test was copied!" << std::endl;
    }

    Test(Test &&rhs) noexcept { // steal resources
        x = rhs.x;
        y = std::move(rhs.y);
        std::cout << "Test was moved!" << std::endl;
    }

    friend std::ostream &operator<<(std::ostream &os, const Test &test) {
        os << "x: " << test.x << " y: " << test.y.size();
        return os;
    }
};

#endif //MODERN_CPP_TEST_H