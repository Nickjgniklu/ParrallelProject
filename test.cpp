#include "InfInt.h"
#include <boost/multiprecision/cpp_bin_float.hpp>
#include <math.h>
#include <iostream>
#include <string>

using namespace boost::multiprecision;
#define BigFloat number<cpp_bin_float<1000, digit_base_10, std::allocator<void>>>

BigFloat square5 = std::sqrt(5);
BigFloat phi1 = (1 + square5) / (2);
BigFloat phi2 = (1 - square5) / (2);

BigFloat fib(int n)
{
    return (pow(phi1, n) - pow(phi2, n)) / square5;
}

int main(int argc, char **)
{
    InfInt bigInt = "123456789354321498765432165498746513549846";
    bigInt *= 2;
    std::cout << bigInt << std::endl;
    BigFloat test = 1.1;
    for (int i = 0; i < 10; i++)
    {
        // std::cout << std::setprecision(1000) << test << std::endl;
        test *= 5.2;
    }
    std::string largeString = test.str();
    std::cout << largeString << std::endl;
}
