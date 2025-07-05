#include <iostream>
#include <map>
#include <string>
#include "mat/Math.hpp"

#include <chrono>

// g++ test.cpp -o test.exe
// g++ test.cpp -o test.exe -O3 --std=c++20 -ftree-vectorize -march=native

using namespace mat;


int main(int argc, char* argv[]){

    // Test base vector class
    Matrix<double, 2, 3> m1{1,2,3,4,5,6};
    Vector<float, 2> v1{1,2};

    std::cout << m1 << std::endl;

    save_bin(m1, "M_1.out");
    save_bin(v1, "V_1.out");

    Matrix<double, 2, 3> m2;
    load_bin(m2, "M_1.out");

    std::cout << m2 << std::endl;

    Vector<float, 2> v2;
    load_bin(v2, "V_1.out");

    std::cout << v2 << std::endl;

    std::cout << "End program" << std::endl;
    return 0;
}