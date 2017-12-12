#ifndef TEST_VECFIELDOPERATIONS_H
#define TEST_VECFIELDOPERATIONS_H

#include "gtest/gtest.h"

#include "utilities/constants.h"
#include "utilities/hybridenums.h"
#include "utilities/types.h"

#include "data/vecfield/vecfield.h"


class UniformVectorFunction : public VectorFunction
{
private:
    double vx_;
    double vy_;
    double vz_;

public:
    UniformVectorFunction(std::array<double, 3> unifArray)
        : vx_{unifArray[0]}
        , vy_{unifArray[1]}
        , vz_{unifArray[2]}
    {
    }

    virtual std::array<double, 3> operator()(double x, double y, double z) override
    {
        (void)x;
        (void)y;
        (void)z;
        return {{vx_, vy_, vz_}};
    }

    virtual ~UniformVectorFunction() = default;
};


struct basicVecFieldParams
{
    AllocSizeT xComponent; // we use the same for the y and z components

    HybridQuantity componentType;
    std::array<std::string, 3> vf_names;

    // Each component is initialized with
    // a distinct uniform field at t1 and t2
    std::array<double, 3> uniformValues_t1;
    std::array<double, 3> uniformValues_t2;

    // expected average
    std::array<double, 3> unif_expected;

    basicVecFieldParams() = default;

    basicVecFieldParams(AllocSizeT xCompo, HybridQuantity compoType,
                        std::array<std::string, 3> names, std::array<double, 3> unif_t1,
                        std::array<double, 3> unif_t2, std::array<double, 3> expected)
        : xComponent{xCompo}
        , componentType{compoType}
        , vf_names{names}
        , uniformValues_t1{unif_t1}
        , uniformValues_t2{unif_t2}
        , unif_expected{expected}
    {
    }
};



struct getVariationParams
{
    basicVecFieldParams vfPars;

    double dt;

    getVariationParams() = default;

    getVariationParams(AllocSizeT xCompo, HybridQuantity compoType,
                       std::array<std::string, 3> names, std::array<double, 3> unif_t1,
                       std::array<double, 3> unif_t2, std::array<double, 3> expected, double dt)
        : vfPars{xCompo, compoType, names, unif_t1, unif_t2, expected}
        , dt{dt}
    {
    }
};



struct timeInterpolationParams
{
    basicVecFieldParams vfPars;

    double dt, delta;

    timeInterpolationParams() = default;

    timeInterpolationParams(AllocSizeT xCompo, HybridQuantity compoType,
                            std::array<std::string, 3> names, std::array<double, 3> unif_t1,
                            std::array<double, 3> unif_t2, std::array<double, 3> expected,
                            double dt, double delta)
        : vfPars{xCompo, compoType, names, unif_t1, unif_t2, expected}
        , dt{dt}
        , delta{delta}
    {
    }
};


// --------------------------------------------------------
//           TIME AVERAGE : TEST CLASS
//
// --------------------------------------------------------
class test_timeAverage : public ::testing::TestWithParam<basicVecFieldParams>
{
public:
    basicVecFieldParams inputs;

    const double precision = std::numeric_limits<float>::epsilon();

    VecField expected_average;
    VecField actual_average;

    test_timeAverage() = default;

    ~test_timeAverage() = default;

    void SetUp();
};

// --------------------------------------------------------
//           GET VARIATION : TEST CLASS
//
// --------------------------------------------------------
class test_getVariation : public ::testing::TestWithParam<getVariationParams>
{
public:
    getVariationParams inputs;

    const double precision = std::numeric_limits<float>::epsilon();

    VecField expected_variation;
    VecField actual_variation;

    test_getVariation() = default;

    ~test_getVariation() = default;

    void SetUp();
};

// --------------------------------------------------------
//           TIME INTERPOLATION : TEST CLASS
//
// --------------------------------------------------------
class test_timeInterpolation : public ::testing::TestWithParam<timeInterpolationParams>
{
public:
    timeInterpolationParams inputs;

    const double precision = std::numeric_limits<float>::epsilon();

    VecField expected_interpolation;
    VecField actual_interpolation;

    test_timeInterpolation() = default;

    ~test_timeInterpolation() = default;

    void SetUp();
};


#endif // TEST_VECFIELDOPERATIONS_H
