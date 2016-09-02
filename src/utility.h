#ifndef UTILITY_H
#define UTILITY_H

#include <limits>
#include <cmath>

namespace utils
{
    inline bool isZero(double value)
     {
         return std::abs( value - std::numeric_limits<double>::epsilon() )
                 <= std::numeric_limits<double>::epsilon();
    }




}



#endif // UTILITY_H
