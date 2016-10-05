
#include <string>
#include <array>
#include <iostream>



#include "test_gridlayout.h"



class GridLayoutCenteredCoordsTest: public ::testing::TestWithParam<GridLayoutParams>
{
public:
    GridLayoutParams inputs ;

    std::vector<Point>  actual_cellCenteredXCoords ;


    void SetUp()
    {
        GridLayoutParams inputs = GetParam(); // GetParam is from GTEST
        print(inputs) ;

        GridLayout gl{ inputs.dxdydz, inputs.nbrCells, inputs.nbDim, "yee", inputs.interpOrder  };

        uint32 isDual = 1 ;

        uint32 iStart = gl.indexAtMin( QtyCentering::primal, Direction::X ) ;
        uint32 iEnd   = gl.indexAtMax( QtyCentering::primal, Direction::X ) - isDual ;

        uint32 iy = gl.indexAtMin( QtyCentering::primal, Direction::Y ) ;
        uint32 iz = gl.indexAtMin( QtyCentering::primal, Direction::Z ) ;

        for( uint32 ix= iStart ; ix<= iEnd ; ix++ )
        {
            actual_cellCenteredXCoords.push_back( \
                        gl.cellCenteredCoordinates(inputs.origin, ix, iy, iz) ) ;
        }

    }



    void print(GridLayoutParams const& inputs)
    {
        std::cout << "interpOrder : " << inputs.interpOrder
                  << " nbDims   : " << inputs.nbDim
                  << " nbrCells : " << inputs.nbrCells[0] << ", " \
                  << inputs.nbrCells[1] << ", " << inputs.nbrCells[2]
                  << " dxdydz   : " << inputs.dxdydz[0] << ", " \
                  << inputs.dxdydz[1] << ", " << inputs.dxdydz[2]
                  << " " <<  inputs.iqty;
    }

};


/*-----------------------------------------------------
 *
 *        Cell centered X coordinates of cells
 *
 *-----------------------------------------------------*/

TEST_P(GridLayoutCenteredCoordsTest, XCenteredCoords)
{

    EXPECT_THAT( actual_cellCenteredXCoords,
                 ::testing::Pointwise(::testing::DoubleEq, inputs.cellCenteredXCoords) ) ;
}



INSTANTIATE_TEST_CASE_P(GridLayoutTest, GridLayoutCenteredCoordsTest,
                        testing::ValuesIn( getCenteredCoordsInputsFromFile() ) );



