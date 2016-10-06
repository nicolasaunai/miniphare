
#include <string>
#include <array>
#include <iostream>


#include "test_gridlayout.h"


MATCHER_P(DoubleNear, epsilon, "Precision out of range")
{
    // we get the actual value
    double actual = std::get<0>(arg) ;

    // we get the expected value
    double expected = std::get<1>(arg) ;

    return actual > expected-epsilon && actual < expected+epsilon ;
}


class GridLayoutCenteredCoordsTest: public ::testing::TestWithParam<GridLayoutParams>
{
public:
    GridLayoutParams inputs ;

    // 2.e-7 is greater than
    // std::numeric_limits<float>::epsilon()
    const double dbl_epsilon = 2.e-7 ;

    std::vector<double>  actual_cellCentersX ;
    std::vector<double>  expected_cellCentersX ;


    void SetUp()
    {
        inputs = GetParam();
        print(inputs) ;

        GridLayout gl{ inputs.dxdydz, inputs.nbrCells, inputs.nbDim, "yee", inputs.interpOrder  };

        uint32 isDual = 1 ;

        uint32 iStart = gl.indexAtMin( QtyCentering::primal, Direction::X ) ;
        uint32 iEnd   = gl.indexAtMax( QtyCentering::primal, Direction::X ) - isDual ;

        uint32 iy = gl.indexAtMin( QtyCentering::primal, Direction::Y ) ;
        uint32 iz = gl.indexAtMin( QtyCentering::primal, Direction::Z ) ;

        for( uint32 ix= iStart ; ix<= iEnd ; ix++ )
        {
            Point cellCenter = gl.cellCenteredCoordinates(inputs.origin, ix, iy, iz) ;

            actual_cellCentersX.push_back( cellCenter.x_ ) ;

            expected_cellCentersX.push_back( inputs.cellCenteredXCoords[ix] ) ;
        }

    }



    void print(GridLayoutParams const& inputs)
    {
        std::cout << " interpOrder : " << inputs.interpOrder
                  << " nbDims   : " << inputs.nbDim
                  << " (uint32) Hybridqty  : " << inputs.iqty << "\n"
                  << " dbl_epsilon : " << dbl_epsilon << "\n"
                  << " nbrCells : " << inputs.nbrCells[0] << ", " \
                  << inputs.nbrCells[1] << ", " << inputs.nbrCells[2] << "\n"
                  << " dxdydz   : " << inputs.dxdydz[0] << ", " \
                  << inputs.dxdydz[1] << ", " << inputs.dxdydz[2] << "\n"
                  << std::endl  ;
    }

};


/*-----------------------------------------------------
 *
 *        Cell centered X coordinates of cells
 *
 *-----------------------------------------------------*/

TEST_P(GridLayoutCenteredCoordsTest, XCenteredCoords)
{
    EXPECT_THAT( actual_cellCentersX, \
                 ::testing::Pointwise(DoubleNear(dbl_epsilon), expected_cellCentersX) ) ;
}



INSTANTIATE_TEST_CASE_P(GridLayoutTest, GridLayoutCenteredCoordsTest,
                        testing::ValuesIn( getCenteredCoordsInputsFromFile() ) );



