
#include <array>
#include <iostream>
#include <string>



#include "test_gridlayout.h"


class GridLayoutIndexingTest : public ::testing::TestWithParam<GridLayoutParams>
{
public:
    GridLayoutParams inputs;

    std::array<uint32, 3> actual_PSI;
    std::array<uint32, 3> actual_PEI;
    std::array<uint32, 3> actual_GSI;
    std::array<uint32, 3> actual_GEI;

    void SetUp()
    {
        inputs = GetParam(); // GetParam is from GTEST
        print(inputs);

        GridLayout gl{inputs.dxdydz, inputs.nbrCells,   inputs.nbDim,
                      "yee",         Point{0., 0., 0.}, inputs.interpOrder};

        // Here the Field sizes for allocations are overestimated
        // We are not testing the alloc methods but only the methods devoted
        // to the determination of indexes
        AllocSizeT allocSize{2 * inputs.nbrCells[0], 2 * inputs.nbrCells[1],
                             2 * inputs.nbrCells[2]};

        Field field{allocSize, inputs.qty, "testField"}; // this is the important parameter here

        actual_PSI[0] = gl.physicalStartIndex(field, Direction::X);
        actual_PSI[1] = gl.physicalStartIndex(field, Direction::Y);
        actual_PSI[2] = gl.physicalStartIndex(field, Direction::Z);

        actual_PEI[0] = gl.physicalEndIndex(field, Direction::X);
        actual_PEI[1] = gl.physicalEndIndex(field, Direction::Y);
        actual_PEI[2] = gl.physicalEndIndex(field, Direction::Z);

        actual_GSI[0] = gl.ghostStartIndex(field, Direction::X);
        actual_GSI[1] = gl.ghostStartIndex(field, Direction::Y);
        actual_GSI[2] = gl.ghostStartIndex(field, Direction::Z);

        actual_GEI[0] = gl.ghostEndIndex(field, Direction::X);
        actual_GEI[1] = gl.ghostEndIndex(field, Direction::Y);
        actual_GEI[2] = gl.ghostEndIndex(field, Direction::Z);
    }


    void print(GridLayoutParams const& inputs)
    {
        std::cout << "interpOrder : " << inputs.interpOrder << " nbDims   : " << inputs.nbDim
                  << " (uint32) Hybridqty  : " << inputs.iqty << "\n"
                  << " nbrCells : " << inputs.nbrCells[0] << ", " << inputs.nbrCells[1] << ", "
                  << inputs.nbrCells[2] << "\n"
                  << " dxdydz   : " << inputs.dxdydz[0] << ", " << inputs.dxdydz[1] << ", "
                  << inputs.dxdydz[2] << "\n"
                  << " PSI x : " << inputs.PSI[0] << "\t PEI x : " << inputs.PEI[0] << "\n"
                  << " GSI x : " << inputs.GSI[0] << "\t GEI x : " << inputs.GEI[0] << "\n"
                  << " PSI y : " << inputs.PSI[1] << "\t PEI y : " << inputs.PEI[1] << "\n"
                  << " GSI y : " << inputs.GSI[1] << "\t GEI y : " << inputs.GEI[1] << "\n"
                  << " PSI z : " << inputs.PSI[2] << "\t PEI z : " << inputs.PEI[2] << "\n"
                  << " GSI z : " << inputs.GSI[2] << "\t GEI z : " << inputs.GEI[2] << std::endl;
    }
};



/*-----------------------------------------------------------------------------
 *
 *
 *                          PHYSICALSTARTINDEX X,Y,Z
 *
 *
 *----------------------------------------------------------------------------*/

TEST_P(GridLayoutIndexingTest, PhysicalStartIndexX)
{
    EXPECT_EQ(inputs.PSI[0], actual_PSI[0]);
}


TEST_P(GridLayoutIndexingTest, PhysicalStartIndexY)
{
    EXPECT_EQ(inputs.PSI[1], actual_PSI[1]);
}


TEST_P(GridLayoutIndexingTest, PhysicalStartIndexZ)
{
    EXPECT_EQ(inputs.PSI[2], actual_PSI[2]);
}

/*-----------------------------------------------------------------------------
 *
 *
 *                          PHYSICALENDINDEX X,Y,Z
 *
 *
 *----------------------------------------------------------------------------*/

TEST_P(GridLayoutIndexingTest, PhysicalEndIndexX)
{
    EXPECT_EQ(inputs.PEI[0], actual_PEI[0]);
}


TEST_P(GridLayoutIndexingTest, PhysicalEndIndexY)
{
    EXPECT_EQ(inputs.PEI[1], actual_PEI[1]);
}


TEST_P(GridLayoutIndexingTest, PhysicalEndIndexZ)
{
    EXPECT_EQ(inputs.PEI[2], actual_PEI[2]);
}


/*-----------------------------------------------------------------------------
 *
 *
 *                          GHOSTSTARTINDEX X,Y,Z
 *
 *
 *----------------------------------------------------------------------------*/

TEST_P(GridLayoutIndexingTest, GhostStartIndexX)
{
    EXPECT_EQ(inputs.GSI[0], actual_GSI[0]);
}


TEST_P(GridLayoutIndexingTest, GhostStartIndexY)
{
    EXPECT_EQ(inputs.GSI[1], actual_GSI[1]);
}


TEST_P(GridLayoutIndexingTest, GhostStartIndexZ)
{
    EXPECT_EQ(inputs.GSI[2], actual_GSI[2]);
}

/*-----------------------------------------------------------------------------
 *
 *
 *                          GHOSTENDINDEX X,Y,Z
 *
 *
 *----------------------------------------------------------------------------*/

TEST_P(GridLayoutIndexingTest, GhostEndIndexX)
{
    EXPECT_EQ(inputs.GEI[0], actual_GEI[0]);
}

TEST_P(GridLayoutIndexingTest, GhostEndIndexY)
{
    EXPECT_EQ(inputs.GEI[1], actual_GEI[1]);
}

TEST_P(GridLayoutIndexingTest, GhostEndIndexZ)
{
    EXPECT_EQ(inputs.GEI[2], actual_GEI[2]);
}



INSTANTIATE_TEST_CASE_P(GridLayoutTest, GridLayoutIndexingTest,
                        testing::ValuesIn(getIndexingInputsFromFile()));
