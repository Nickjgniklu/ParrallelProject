#include "InfInt.h"
#include <vector>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <boost/multiprecision/cpp_bin_float.hpp>

using namespace boost::multiprecision;
#define BigFloat number<cpp_bin_float<1000, digit_base_10, std::allocator<void>>>

BigFloat square5 = std::sqrt(5);
BigFloat phi1 = (1 + square5) / (2);
BigFloat phi2 = (1 - square5) / (2);

BigFloat fib(int n)
{
    return (pow(phi1, n) - pow(phi2, n)) / square5;
}

int main()
{
    int iterations = 10000;
    InfInt previous1 = 1;
    InfInt previous2 = 1;
    auto before = std::chrono::high_resolution_clock::now();
    for (int iterator = 2; iterator <= iterations / 5; iterator++)
    {
        InfInt newValue = previous1 + previous2;
        previous2 = previous1;
        previous1 = newValue;
    }
    std::cout << previous1 << std::endl;
    auto after = std::chrono::high_resolution_clock::now();
    std::cout << "Serial calulation for " << iterations << " iterations with the While Loop method: " << std::chrono::duration_cast<std::chrono::duration<float>>(after - before).count() << " seconds" << std::endl;

    before = std::chrono::high_resolution_clock::now();
    // BigFloat result;
    // for (int iterator = 1; iterator <= iterations; iterator++)
    // {
    BigFloat result = (pow(phi1, iterations / 5) - pow(phi2, iterations / 5)) / square5;
    // (pow(phi1, iterator) - pow(phi2, iterator)) / square5;
    // std::cout << std::setprecision(1000) <<  result;
    // }
    std::cout << result.str() << std::endl;
    after = std::chrono::high_resolution_clock::now();
    std::cout << "Serial calulation for " << iterations << " iterations with the Formula method: " << std::chrono::duration_cast<std::chrono::duration<float>>(after - before).count() << " seconds" << std::endl;
    return 0;
}