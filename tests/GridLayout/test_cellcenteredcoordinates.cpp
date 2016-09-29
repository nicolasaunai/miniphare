
#include <string>
#include <array>
#include <iostream>



#include "test_gridlayout.h"



/*-----------------------------------------------------
 *
 *        Cell centered X coordinates of cells
 *
 *-----------------------------------------------------*/

TEST_P(GridLayoutCenteredCoordsTest, XCenteredCoords)
{
    GridLayoutParams inputs = GetParam(); // GetParam is from GTEST
    std::cout << inputs  << std::endl;

    GridLayout gl{ inputs.dxdydz, inputs.nbrCells, inputs.nbDim, "yee", inputs.interpOrder  };

    // Here the Field sizes for allocations are overestimated
    AllocSizeT allocSize{2*inputs.nbrCells[0],2*inputs.nbrCells[1],2*inputs.nbrCells[2]};

    // inputs.qty is the important parameter
    Field field{allocSize , inputs.qty, "testField" };

    uint32 isDual = 1 ;

    uint32 iStart = gl.indexAtMin( QtyCentering::primal, Direction::X ) ;
    uint32 iEnd   = gl.indexAtMax( QtyCentering::primal, Direction::X ) - isDual ;

    uint32 iy = gl.indexAtMin( QtyCentering::primal, Direction::Y ) ;
    uint32 iz = gl.indexAtMin( QtyCentering::primal, Direction::Z ) ;


    for( uint32 ix= iStart ; ix<= iEnd ; ix++ )
    {
        Point ptest = gl.cellCenteredCoordinates(inputs.origin, ix, iy, iz) ;

        ASSERT_DOUBLE_EQ( inputs.cellCenteredXCoords[ix], ptest.x_ ) ;
    }

}



INSTANTIATE_TEST_CASE_P(GridLayoutTest, GridLayoutCenteredCoordsTest,
                        testing::ValuesIn( getCenteredCoordsInputsFromFile() ) );



