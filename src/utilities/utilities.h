#ifndef UTILITIES_H
#define UTILITIES_H

#include <vector>

template<typename T>
std::vector<T> arange(T start, T stop, T step = 1)
{
    std::vector<T> values;
    for (T value = start; value < stop; value += step)
        values.push_back(value);
    return values;
}


#endif // UTILITIES_H
