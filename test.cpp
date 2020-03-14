#include <stdio.h>
#include <math.h>
#include <iostream>
#include <iomanip>
using namespace std;
double square5 = sqrt(5);
double phi = (1 + square5) / (2);
double nphi = (1 - square5) / (2);

double fib(int n)
{
    return round((pow(phi, n) - pow(nphi, n)) / square5);
}
int main(int argc, char **)
{
    for (int i = 0; i < 2000; i++)
    {
        cout << fixed << "fib " << i << ":" << fib(i)<<endl;
    }
    return 0;
}