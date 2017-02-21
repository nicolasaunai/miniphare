#ifndef TYPES_H
#define TYPES_H

#include <cinttypes>
#include <array>

using uint32 = std::uint32_t;
using uint64 = std::uint64_t;
using int32  = std::int32_t;
using int64  = std::int64_t;


using ScalarFunction = double (*) (double x, double y, double z);
using VectorFunction =  std::array<double,3> (*) (double x, double y, double z);


enum class Edge{ Xmin, Xmax, Ymin, Ymax, Zmin, Zmax } ;

struct AllocSizeT{

    uint32 nx_ ;
    uint32 ny_ ;
    uint32 nz_ ;

    AllocSizeT( uint32 nx , uint32 ny,
               uint32 nz )
        : nx_(nx), ny_(ny), nz_(nz)
    {
    }

};


struct Point
{
    double x_ = 0.;
    double y_ = 0.;
    double z_ = 0.;

    Point() = default;
    Point(double x, double y, double z):
        x_{x}, y_{y}, z_{z} {}

} ;



#endif // TYPES_H
