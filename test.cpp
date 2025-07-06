#include <iostream>
#include <map>
#include <string>
#include "mat/Math.hpp"

#include <chrono>
#include <random>

// g++ test.cpp -o test.exe
// g++ test.cpp -o test.exe -O3 --std=c++20 -ftree-vectorize -march=native

using namespace mat;


int main(int argc, char* argv[]){

    BaseVector<float, 2> bv1{3.1, 6.5};
    Vector<float,2> v1 = bv1;
    Complex<float> c1 = v1;

    std::cout << bv1 << std::endl << v1 << std::endl << c1 << std::endl;

    std::random_device rd;                         // Seed
    std::mt19937 gen(rd());                        // Mersenne Twister engine
    std::uniform_real_distribution<double> dist(0, 100);  // Range: [0.0, 1.0)

    Matrix<double, 12, 100> m1;
    for (uint32_t i(0) ; i < m1.row() ; ++i) {
        for (uint32_t j(0) ; j < m1.column() ; ++j) {
            m1(i,j) = (int)dist(gen);
        }
    }

    save_txt(m1, "big_m.out", ' ', 10);
    Matrix<double, 12, 100> m2;
    load_txt(m2, "big_m.out");

    if (m1 == m2) {
        std::cout << "Serialization complete" << std::endl;
    }else{
        std::cout << "ERROR" << std::endl;
    }

    std::cout << "End program" << std::endl;
    return 0;
}