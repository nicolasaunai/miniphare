#ifndef UTILITY_H
#define UTILITY_H

#include <limits>
#include <cmath>

namespace utils
{
    inline bool isEqual(double value1, double value2,
                        double absErr = std::numeric_limits<double>::epsilon())
     {
         return std::abs( value1 - value2 )
                 <= absErr;
    }




}



#endif // UTILITY_H
