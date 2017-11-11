#ifndef TYPES_H
#define TYPES_H

#include <array>
#include <cinttypes>

using uint32 = std::uint32_t;
using uint64 = std::uint64_t;
using int32  = std::int32_t;
using int64  = std::int64_t;


// using ScalarFunction = double (*)(double x, double y, double z);
// using VectorFunction = std::array<double, 3> (*)(double x, double y, double z);

/**
 * @brief represents a function of 3D spatial coordinates returning a scalar
 */
class ScalarFunction
{
public:
    virtual double operator()(double x, double y, double z) = 0;

    virtual ~ScalarFunction() = default;
};


/**
 * @brief represents a function of 3D spatial coordinates returning a vector
 */
class VectorFunction
{
public:
    virtual std::array<double, 3> operator()(double x, double y, double z) = 0;

    virtual ~VectorFunction() = default;
};



enum class Edge { Xmin, Xmax, Ymin, Ymax, Zmin, Zmax };

struct AllocSizeT
{
    uint32 nx_ = 0;
    uint32 ny_ = 0;
    uint32 nz_ = 0;

    AllocSizeT() = default;

    AllocSizeT(uint32 nx, uint32 ny, uint32 nz)
        : nx_(nx)
        , ny_(ny)
        , nz_(nz)
    {
    }
};


struct Point
{
    double x = 0.;
    double y = 0.;
    double z = 0.;

    Point() = default;
    Point(double x, double y, double z)
        : x{x}
        , y{y}
        , z{z}
    {
    }

    double getCoord(uint32 idim)
    {
        double value = 0.;
        switch (idim)
        {
            case 0: value = x; break;

            case 1: value = y; break;

            case 2: value = z; break;

            default: throw std::runtime_error("wrong dimensionality");
        }

        return value;
    }
};



#endif // TYPES_H
