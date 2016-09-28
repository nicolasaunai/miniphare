
#include <string>
#include <array>
#include <iostream>



#include "test_gridlayout.h"



/*-----------------------------------------------------------------------------
 *
 *
 *                    X coordinates for
 *        - Bx, By, Bz, Ex, Ey, Ez, rho, V, P  fields
 *
 *----------------------------------------------------------------------------*/




TEST_P(GridLayoutFieldCoordsTest, XCoordsBx)
{
    GridLayoutParams inputs = GetParam(); // GetParam is from GTEST
    std::cout << inputs  << std::endl;

    GridLayout gl{ inputs.dxdydz, inputs.nbrCells, inputs.nbDim, "yee", inputs.interpOrder  };

    // Here the Field sizes for allocations are overestimated
    AllocSizeT allocSize{2*inputs.nbrCells[0],2*inputs.nbrCells[1],2*inputs.nbrCells[2]};

    // inputs.qty is the important parameter
    Field field{allocSize , inputs.qty, "testField" };

//    EXPECT_EQ(inputs.PSI[0], gl.physicalStartIndex( field, Direction::X) );
}







INSTANTIATE_TEST_CASE_P(GridLayoutTest, GridLayoutFieldCoordsTest,
                        testing::ValuesIn( getFieldCoordsInputsFromFile() ) );



