
#include <array>
#include <memory>
#include <vector>

#include <utilities/utilities.h>

#include "gmock/gmock.h"
#include "gtest/gtest.h"



TEST(test_utilities, basisVectorsAreNormalized)
{
    std::array<double, 3> vec{{1, 2, 3}};
    std::array<std::array<double, 3>, 3> basis;

    localMagneticBasis(vec, basis);

    for (auto comp = 0u; comp < 3; ++comp)
    {
        auto norm = basis[comp][0] * basis[comp][0] + basis[comp][1] * basis[comp][1]
                    + basis[comp][2] * basis[comp][2];
        ASSERT_DOUBLE_EQ(1, norm);
    }
}



TEST(test_utilities, basisIsOrthogonal)
{
    std::array<double, 3> vec{{1, 2, 3}};
    std::array<std::array<double, 3>, 3> basis;

    localMagneticBasis(vec, basis);

    EXPECT_TRUE(1e-12 > basis[0][0] * basis[1][0] + basis[0][1] * basis[1][1]
                            + basis[0][2] * basis[1][2]);
    EXPECT_TRUE(1e-12 > basis[1][0] * basis[2][0] + basis[1][1] * basis[2][1]
                            + basis[1][2] * basis[2][2]);
    EXPECT_TRUE(1e-12 > basis[0][0] * basis[2][0] + basis[0][1] * basis[2][1]
                            + basis[0][2] * basis[2][2]);
}




int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
