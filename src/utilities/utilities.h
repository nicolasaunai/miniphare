#ifndef UTILITIES_H
#define UTILITIES_H

#include <array>
#include <cmath>
#include <numeric>
#include <utilities/types.h>
#include <vector>


template<typename T>
std::vector<T> arange(T start, T stop, T step = 1)
{
    std::vector<T> values;
    for (T value = start; value < stop; value += step)
        values.push_back(value);
    return values;
}

template<typename T>
T norm(std::array<T, 3> vec)
{
    auto squarreSum = std::inner_product(std::begin(vec), std::end(vec), std::begin(vec), 0.);
    return std::sqrt(squarreSum);
}


std::vector<uint32> stripStringToVector(std::string str);


void localMagneticBasis(std::array<double, 3> B, std::array<std::array<double, 3>, 3>& basis);



std::array<double, 3> basisTransform(const std::array<std::array<double, 3>, 3> basis,
                                     std::array<double, 3> vec);


#endif // UTILITIES_H
