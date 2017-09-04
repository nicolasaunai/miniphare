#ifndef UTILITYPHARE_H
#define UTILITYPHARE_H

#include <cmath>
#include <limits>
#include <vector>

#include "types.h"


namespace utils
{
inline bool isEqual(double value1, double value2,
                    double absErr = std::numeric_limits<double>::epsilon())
{
    return std::abs(value1 - value2) <= absErr;
}
}


struct Box
{
    double x0, x1, y0, y1, z0, z1;

    Box()
        : x0{0.}
        , x1{0.}
        , y0{0.}
        , y1{0.}
        , z0{0.}
        , z1{0.}
    {
    }

    Box(double xmin, double xmax)
        : x0{xmin}
        , x1{xmax}
        , y0{0.}
        , y1{0.}
        , z0{0.}
        , z1{0.}
    {
    }

    Box(double xmin, double xmax, double ymin, double ymax)
        : x0{xmin}
        , x1{xmax}
        , y0{ymin}
        , y1{ymax}
        , z0{0.}
        , z1{0.}
    {
    }

    Box(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax)
        : x0{xmin}
        , x1{xmax}
        , y0{ymin}
        , y1{ymax}
        , z0{zmin}
        , z1{zmax}
    {
    }

    void expand(std::array<double, 3> padding)
    {
        x0 -= padding[0];
        x1 += padding[0];
        y0 -= padding[1];
        y1 += padding[1];
        z0 -= padding[2];
        z1 += padding[2];
    }
};


/**
 * @brief The LogicalBox struct only stores indexes
 * Remember that indexes are relative to a
 * given GridLayout (i.e. a given patch)
 *
 * We have no information about the patch position
 * in the whole domain
 *
 */
struct LogicalBox
{
    int32 ix0 = 1, ix1 = 1, iy0 = 1, iy1 = 1, iz0 = 1, iz1 = 1;

    LogicalBox() {}

    LogicalBox(int32 ix0, int32 ix1)
        : ix0{ix0}
        , ix1{ix1}
    {
    }

    LogicalBox(int32 ix0, int32 ix1, int32 iy0, int32 iy1)
        : ix0{ix0}
        , ix1{ix1}
        , iy0{iy0}
        , iy1{iy1}
    {
    }

    LogicalBox(int32 ix0, int32 ix1, int32 iy0, int32 iy1, int32 iz0, int32 iz1)
        : ix0{ix0}
        , ix1{ix1}
        , iy0{iy0}
        , iy1{iy1}
        , iz0{iz0}
        , iz1{iz1}
    {
    }
};



#endif // UTILITYPHARE_H
