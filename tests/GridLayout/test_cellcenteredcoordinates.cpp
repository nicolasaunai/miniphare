
#include <array>
#include <iostream>
#include <limits>
#include <string>


#include "test_gridlayout.h"



class GridLayoutCenteredCoordsTest : public ::testing::TestWithParam<GridLayoutParams>
{
public:
    GridLayoutParams inputs;

    // 2.e-7 is greater than
    // std::numeric_limits<float>::epsilon()
    const double dbl_epsilon = std::numeric_limits<float>::epsilon();

    std::vector<double> actual_cellCentersX;
    std::vector<double> expected_cellCentersX;


    void SetUp()
    {
        inputs = GetParam();
        print(inputs);

        GridLayout gl{inputs.dxdydz, inputs.nbrCells, inputs.nbDim,
                      "yee",         inputs.origin,   inputs.interpOrder};

        uint32 isDual = 1;

        uint32 iStart = gl.physicalStartIndex(QtyCentering::primal, Direction::X);
        uint32 iEnd   = gl.physicalEndIndex(QtyCentering::primal, Direction::X) - isDual;

        uint32 iy = gl.physicalStartIndex(QtyCentering::primal, Direction::Y);
        uint32 iz = gl.physicalStartIndex(QtyCentering::primal, Direction::Z);

        for (uint32 ix = iStart; ix <= iEnd; ix++)
        {
            Point cellCenter = gl.cellCenteredCoordinates(ix, iy, iz);

            actual_cellCentersX.push_back(cellCenter.x);

            expected_cellCentersX.push_back(inputs.cellCenteredXCoords[ix]);
        }
    }



    void print(GridLayoutParams const& inputs)
    {
        std::cout << " interpOrder : " << inputs.interpOrder << " nbDims   : " << inputs.nbDim
                  << " (uint32) Hybridqty  : " << inputs.iqty << "\n"
                  << " dbl_epsilon : " << dbl_epsilon << "\n"
                  << " nbrCells : " << inputs.nbrCells[0] << ", " << inputs.nbrCells[1] << ", "
                  << inputs.nbrCells[2] << "\n"
                  << " dxdydz   : " << inputs.dxdydz[0] << ", " << inputs.dxdydz[1] << ", "
                  << inputs.dxdydz[2] << "\n"
                  << std::endl;
    }
};


/*-----------------------------------------------------
 *
 *        Cell centered X coordinates of cells
 *
 *-----------------------------------------------------*/

TEST_P(GridLayoutCenteredCoordsTest, XCenteredCoords)
{
    EXPECT_THAT(actual_cellCentersX,
                ::testing::Pointwise(DoubleNear(dbl_epsilon), expected_cellCentersX));
}



INSTANTIATE_TEST_CASE_P(GridLayoutTest, GridLayoutCenteredCoordsTest,
                        testing::ValuesIn(getCenteredCoordsInputsFromFile()));
