
#include <string>
#include <array>
#include <iostream>
#include <fstream>




#include "test_gridlayout.h"




void testFieldNodeCoordinates( GridLayoutParams & inputs,
                               const GridLayout & gl     )
{

    // Here the Field sizes for allocations are overestimated
    AllocSizeT allocSize{2*inputs.nbrCells[0],2*inputs.nbrCells[1],2*inputs.nbrCells[2]};

    // inputs.qty is the important parameter
    Field field{allocSize , inputs.qty, "testField" };

    uint32 iy = gl.indexAtMin( QtyCentering::primal, Direction::Y ) ;
    uint32 iz = gl.indexAtMin( QtyCentering::primal, Direction::Z ) ;

    // End of input reading in the adequate file
    uint32 order = inputs.interpOrder ;
    uint32 icase = inputs.icase ;
    uint32 dim = inputs.nbDim ;

    std::string qtyName = inputs.qtyName;

    std::string filename{"../GridLayout/fieldCoords_ord" + std::to_string(order) +
                "_dim" + std::to_string(dim) + "_" + qtyName +
                "_case" + std::to_string(icase) + ".txt"};

    std::cout << filename << std::endl ;

    std::ifstream ifs2{filename};
    if (!ifs2 )
    {
        std::cout << "Could not open file : " << filename << std::endl ;
        exit(-1);
    }

    uint32 iStart = inputs.field_iStart ;
    uint32 iEnd   = inputs.field_iEnd   ;
    for (uint32 ik=iStart ; ik<iEnd+1 ; ++ik)
    {
        ifs2 >> inputs.fieldXCoords[ik] ;
    }


    for( uint32 ix= iStart ; ix<= iEnd ; ix++ )
    {
        Point ptest = gl.fieldNodeCoordinates(field, inputs.origin, ix, iy, iz) ;

        ASSERT_DOUBLE_EQ( inputs.fieldXCoords[ix], ptest.x_ ) ;
    }


}


/*-----------------------------------------------------
 *        X coordinates for fields:
 *        - Bx, By, Bz
 *
 *-----------------------------------------------------*/

TEST_P(GridLayoutFieldCoordsTest, XCoordsBx)
{
    GridLayoutParams inputs = GetParam();
    std::cout << inputs  << std::endl;

    GridLayout gl{ inputs.dxdydz, inputs.nbrCells, inputs.nbDim, "yee", inputs.interpOrder  };

    // This test is only for Bx field
    if( inputs.qtyName == "Bx" )
    {
        testFieldNodeCoordinates( inputs, gl ) ;
    }

}



TEST_P(GridLayoutFieldCoordsTest, XCoordsBy)
{
    GridLayoutParams inputs = GetParam();
    std::cout << inputs  << std::endl;

    GridLayout gl{ inputs.dxdydz, inputs.nbrCells, inputs.nbDim, "yee", inputs.interpOrder  };

    // This test is only for By field
    if( inputs.qtyName == "By" )
    {
        testFieldNodeCoordinates( inputs, gl ) ;
    }

}



TEST_P(GridLayoutFieldCoordsTest, XCoordsBz)
{
    GridLayoutParams inputs = GetParam();
    std::cout << inputs  << std::endl;

    GridLayout gl{ inputs.dxdydz, inputs.nbrCells, inputs.nbDim, "yee", inputs.interpOrder  };

    // This test is only for Bz field
    if( inputs.qtyName == "Bz" )
    {
        testFieldNodeCoordinates( inputs, gl ) ;
    }

}


/*-----------------------------------------------------
 *        X coordinates for fields:
 *        - Ex, Ey, Ez
 *
 *-----------------------------------------------------*/

TEST_P(GridLayoutFieldCoordsTest, XCoordsEx)
{
    GridLayoutParams inputs = GetParam();
    std::cout << inputs  << std::endl;

    GridLayout gl{ inputs.dxdydz, inputs.nbrCells, inputs.nbDim, "yee", inputs.interpOrder  };

    // This test is only for Ex field
    if( inputs.qtyName == "Ex" )
    {
        testFieldNodeCoordinates( inputs, gl ) ;
    }

}


TEST_P(GridLayoutFieldCoordsTest, XCoordsEy)
{
    GridLayoutParams inputs = GetParam();
    std::cout << inputs  << std::endl;

    GridLayout gl{ inputs.dxdydz, inputs.nbrCells, inputs.nbDim, "yee", inputs.interpOrder  };

    // This test is only for Ey field
    if( inputs.qtyName == "Ey" )
    {
        testFieldNodeCoordinates( inputs, gl ) ;
    }

}


TEST_P(GridLayoutFieldCoordsTest, XCoordsEz)
{
    GridLayoutParams inputs = GetParam();
    std::cout << inputs  << std::endl;

    GridLayout gl{ inputs.dxdydz, inputs.nbrCells, inputs.nbDim, "yee", inputs.interpOrder  };

    // This test is only for Ez field
    if( inputs.qtyName == "Ez" )
    {
        testFieldNodeCoordinates( inputs, gl ) ;
    }

}


/*-----------------------------------------------------
 *        X coordinates for fields:
 *        - rho, V, P
 *
 *-----------------------------------------------------*/

TEST_P(GridLayoutFieldCoordsTest, XCoordsrho)
{
    GridLayoutParams inputs = GetParam();
    std::cout << inputs  << std::endl;

    GridLayout gl{ inputs.dxdydz, inputs.nbrCells, inputs.nbDim, "yee", inputs.interpOrder  };

    // This test is only for rho field
    if( inputs.qtyName == "rho" )
    {
        testFieldNodeCoordinates( inputs, gl ) ;
    }

}


TEST_P(GridLayoutFieldCoordsTest, XCoordsV)
{
    GridLayoutParams inputs = GetParam();
    std::cout << inputs  << std::endl;

    GridLayout gl{ inputs.dxdydz, inputs.nbrCells, inputs.nbDim, "yee", inputs.interpOrder  };

    // This test is only for V field
    if( inputs.qtyName == "V" )
    {
        testFieldNodeCoordinates( inputs, gl ) ;
    }

}


TEST_P(GridLayoutFieldCoordsTest, XCoordsP)
{
    GridLayoutParams inputs = GetParam();
    std::cout << inputs  << std::endl;

    GridLayout gl{ inputs.dxdydz, inputs.nbrCells, inputs.nbDim, "yee", inputs.interpOrder  };

    // This test is only for P field
    if( inputs.qtyName == "P" )
    {
        testFieldNodeCoordinates( inputs, gl ) ;
    }

}




INSTANTIATE_TEST_CASE_P(GridLayoutTest, GridLayoutFieldCoordsTest,
                        testing::ValuesIn( getFieldCoordsInputsFromFile() ) );



