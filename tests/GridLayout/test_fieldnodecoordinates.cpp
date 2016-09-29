
#include <string>
#include <array>
#include <iostream>



#include "test_gridlayout.h"



/*-----------------------------------------------------
 *        X coordinates for fields:
 *        - Bx, By, Bz
 *
 *-----------------------------------------------------*/

TEST_P(GridLayoutFieldCoordsTest, XCoordsBx)
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


    // This test is only for Bx field
    if( inputs.qtyName == "Bx" )
    {
        for( uint32 ix= iStart ; ix<= iEnd ; ix++ )
        {
            Point ptest = gl.fieldNodeCoordinates(field, inputs.origin, ix, iy, iz) ;

            ASSERT_DOUBLE_EQ( inputs.fieldXCoords[ix], ptest.x_ ) ;
        }
    }

}



TEST_P(GridLayoutFieldCoordsTest, XCoordsBy)
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


    // This test is only for By field
    if( inputs.qtyName == "By" )
    {
        for( uint32 ix= iStart ; ix<= iEnd ; ix++ )
        {
            Point ptest = gl.fieldNodeCoordinates(field, inputs.origin, ix, iy, iz) ;

            ASSERT_DOUBLE_EQ( inputs.fieldXCoords[ix], ptest.x_ ) ;
        }
    }

}



TEST_P(GridLayoutFieldCoordsTest, XCoordsBz)
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


    // This test is only for Bz field
    if( inputs.qtyName == "Bz" )
    {
        for( uint32 ix= iStart ; ix<= iEnd ; ix++ )
        {
            Point ptest = gl.fieldNodeCoordinates(field, inputs.origin, ix, iy, iz) ;

            ASSERT_DOUBLE_EQ( inputs.fieldXCoords[ix], ptest.x_ ) ;
        }
    }

}


/*-----------------------------------------------------
 *        X coordinates for fields:
 *        - Ex, Ey, Ez
 *
 *-----------------------------------------------------*/

TEST_P(GridLayoutFieldCoordsTest, XCoordsEx)
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


    // This test is only for Ex field
    if( inputs.qtyName == "Ex" )
    {
        for( uint32 ix= iStart ; ix<= iEnd ; ix++ )
        {
            Point ptest = gl.fieldNodeCoordinates(field, inputs.origin, ix, iy, iz) ;

            ASSERT_DOUBLE_EQ( inputs.fieldXCoords[ix], ptest.x_ ) ;
        }
    }

}


TEST_P(GridLayoutFieldCoordsTest, XCoordsEy)
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


    // This test is only for Ey field
    if( inputs.qtyName == "Ey" )
    {
        for( uint32 ix= iStart ; ix<= iEnd ; ix++ )
        {
            Point ptest = gl.fieldNodeCoordinates(field, inputs.origin, ix, iy, iz) ;

            ASSERT_DOUBLE_EQ( inputs.fieldXCoords[ix], ptest.x_ ) ;
        }
    }

}


TEST_P(GridLayoutFieldCoordsTest, XCoordsEz)
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


    // This test is only for Ez field
    if( inputs.qtyName == "Ez" )
    {
        for( uint32 ix= iStart ; ix<= iEnd ; ix++ )
        {
            Point ptest = gl.fieldNodeCoordinates(field, inputs.origin, ix, iy, iz) ;

            ASSERT_DOUBLE_EQ( inputs.fieldXCoords[ix], ptest.x_ ) ;
        }
    }

}


/*-----------------------------------------------------
 *        X coordinates for fields:
 *        - rho, V, P
 *
 *-----------------------------------------------------*/

TEST_P(GridLayoutFieldCoordsTest, XCoordsrho)
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


    // This test is only for rho field
    if( inputs.qtyName == "rho" )
    {
        for( uint32 ix= iStart ; ix<= iEnd ; ix++ )
        {
            Point ptest = gl.fieldNodeCoordinates(field, inputs.origin, ix, iy, iz) ;

            ASSERT_DOUBLE_EQ( inputs.fieldXCoords[ix], ptest.x_ ) ;
        }
    }

}


TEST_P(GridLayoutFieldCoordsTest, XCoordsV)
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


    // This test is only for V field
    if( inputs.qtyName == "V" )
    {
        for( uint32 ix= iStart ; ix<= iEnd ; ix++ )
        {
            Point ptest = gl.fieldNodeCoordinates(field, inputs.origin, ix, iy, iz) ;

            ASSERT_DOUBLE_EQ( inputs.fieldXCoords[ix], ptest.x_ ) ;
        }
    }

}


TEST_P(GridLayoutFieldCoordsTest, XCoordsP)
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


    // This test is only for P field
    if( inputs.qtyName == "P" )
    {
        for( uint32 ix= iStart ; ix<= iEnd ; ix++ )
        {
            Point ptest = gl.fieldNodeCoordinates(field, inputs.origin, ix, iy, iz) ;

            ASSERT_DOUBLE_EQ( inputs.fieldXCoords[ix], ptest.x_ ) ;
        }
    }

}




INSTANTIATE_TEST_CASE_P(GridLayoutTest, GridLayoutFieldCoordsTest,
                        testing::ValuesIn( getFieldCoordsInputsFromFile() ) );



