
#include <string>
#include <array>
#include <iostream>



#include "test_gridlayout.h"



/*-----------------------------------------------------------------------------
 *
 *
 *                          PHYSICALSTARTINDEX X,Y,Z
 *
 *
 *----------------------------------------------------------------------------*/




TEST_P(GridLayoutIndexingTest, PhysicalStartIndexX)
{
    GridLayoutParams inputs = GetParam(); // GetParam is from GTEST
    std::cout << inputs  << std::endl;

    GridLayout gl{ inputs.dxdydz, inputs.nbrCells, inputs.nbDim, "yee", inputs.interpOrder  };

    // Here the Field sizes for allocations are overestimated
    // We are not testing the alloc methods but only the methods devoted
    // to the determination of indexes
    AllocSizeT allocSize{2*inputs.nbrCells[0],2*inputs.nbrCells[1],2*inputs.nbrCells[2]};
    Field field{allocSize, inputs.qty, "testField" }; // this is the important parameter here

    EXPECT_EQ(inputs.PSI[0], gl.physicalStartIndex( field, Direction::X) );
}





TEST_P(GridLayoutIndexingTest, PhysicalStartIndexY)
{
   GridLayoutParams inputs = GetParam(); // GetParam is from GTEST
   std::cout << inputs  << std::endl;
   GridLayout gl{ inputs.dxdydz, inputs.nbrCells, inputs.nbDim, "yee", inputs.interpOrder  };
   AllocSizeT allocSize{2*inputs.nbrCells[0],2*inputs.nbrCells[1],2*inputs.nbrCells[2]};
   Field field{allocSize, inputs.qty, "testField" }; // this is the important parameter here

   EXPECT_EQ(inputs.PSI[1], gl.physicalStartIndex( field, Direction::Y) );
}




TEST_P(GridLayoutIndexingTest, PhysicalStartIndexZ)
{
   GridLayoutParams inputs = GetParam(); // GetParam is from GTEST
   std::cout << inputs  << std::endl;
   GridLayout gl{ inputs.dxdydz, inputs.nbrCells, inputs.nbDim, "yee", inputs.interpOrder  };
   AllocSizeT allocSize{2*inputs.nbrCells[0],2*inputs.nbrCells[1],2*inputs.nbrCells[2]};
   Field field{allocSize, inputs.qty, "testField" }; // this is the important parameter here

   EXPECT_EQ(inputs.PSI[2], gl.physicalStartIndex( field, Direction::Z) );
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
   GridLayoutParams inputs = GetParam(); // GetParam is from GTEST
   std::cout << inputs  << std::endl;
   GridLayout gl{ inputs.dxdydz, inputs.nbrCells, inputs.nbDim, "yee", inputs.interpOrder  };
   AllocSizeT allocSize{2*inputs.nbrCells[0],2*inputs.nbrCells[1],2*inputs.nbrCells[2]};
   Field field{allocSize, inputs.qty, "testField" }; // this is the important parameter here

   EXPECT_EQ(inputs.PEI[0], gl.physicalEndIndex  ( field, Direction::X) );
}




TEST_P(GridLayoutIndexingTest, PhysicalEndIndexY)
{
   GridLayoutParams inputs = GetParam(); // GetParam is from GTEST
   std::cout << inputs  << std::endl;
   GridLayout gl{ inputs.dxdydz, inputs.nbrCells, inputs.nbDim, "yee", inputs.interpOrder  };
   AllocSizeT allocSize{2*inputs.nbrCells[0],2*inputs.nbrCells[1],2*inputs.nbrCells[2]};
   Field field{allocSize, inputs.qty, "testField" }; // this is the important parameter here

   EXPECT_EQ(inputs.PEI[1], gl.physicalEndIndex  ( field, Direction::Y) );
}



TEST_P(GridLayoutIndexingTest, PhysicalEndIndexZ)
{
   GridLayoutParams inputs = GetParam(); // GetParam is from GTEST
   std::cout << inputs  << std::endl;
   GridLayout gl{ inputs.dxdydz, inputs.nbrCells, inputs.nbDim, "yee", inputs.interpOrder  };
   AllocSizeT allocSize{2*inputs.nbrCells[0],2*inputs.nbrCells[1],2*inputs.nbrCells[2]};
   Field field{allocSize, inputs.qty, "testField" }; // this is the important parameter here

   EXPECT_EQ(inputs.PEI[2], gl.physicalEndIndex  ( field, Direction::Z) );
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
   GridLayoutParams inputs = GetParam(); // GetParam is from GTEST
   std::cout << inputs  << std::endl;
   GridLayout gl{ inputs.dxdydz, inputs.nbrCells, inputs.nbDim, "yee", inputs.interpOrder  };
   AllocSizeT allocSize{2*inputs.nbrCells[0],2*inputs.nbrCells[1],2*inputs.nbrCells[2]};
   Field field{allocSize, inputs.qty, "testField" }; // this is the important parameter here

   EXPECT_EQ(inputs.GSI[0], gl.ghostStartIndex   ( field, Direction::X) );
}




TEST_P(GridLayoutIndexingTest, GhostStartIndexY)
{
   GridLayoutParams inputs = GetParam(); // GetParam is from GTEST
   std::cout << inputs  << std::endl;
   GridLayout gl{ inputs.dxdydz, inputs.nbrCells, inputs.nbDim, "yee", inputs.interpOrder  };
   AllocSizeT allocSize{2*inputs.nbrCells[0],2*inputs.nbrCells[1],2*inputs.nbrCells[2]};
   Field field{allocSize, inputs.qty, "testField" }; // this is the important parameter here

   EXPECT_EQ(inputs.GSI[1], gl.ghostStartIndex   ( field, Direction::Y) );
}




TEST_P(GridLayoutIndexingTest, GhostStartIndexZ)
{
   GridLayoutParams inputs = GetParam(); // GetParam is from GTEST
   std::cout << inputs  << std::endl;
   GridLayout gl{ inputs.dxdydz, inputs.nbrCells, inputs.nbDim, "yee", inputs.interpOrder  };
   AllocSizeT allocSize{2*inputs.nbrCells[0],2*inputs.nbrCells[1],2*inputs.nbrCells[2]};
   Field field{allocSize, inputs.qty, "testField" }; // this is the important parameter here

   EXPECT_EQ(inputs.GSI[2], gl.ghostStartIndex   ( field, Direction::Z) );
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
   GridLayoutParams inputs = GetParam(); // GetParam is from GTEST
   std::cout << inputs  << std::endl;
   GridLayout gl{ inputs.dxdydz, inputs.nbrCells, inputs.nbDim, "yee", inputs.interpOrder  };
   AllocSizeT allocSize{2*inputs.nbrCells[0],2*inputs.nbrCells[1],2*inputs.nbrCells[2]};
   Field field{allocSize, inputs.qty, "testField" }; // this is the important parameter here

   EXPECT_EQ(inputs.GEI[0], gl.ghostEndIndex     ( field, Direction::X) );
}



TEST_P(GridLayoutIndexingTest, GhostEndIndexY)
{
   GridLayoutParams inputs = GetParam(); // GetParam is from GTEST
   std::cout << inputs  << std::endl;
   GridLayout gl{ inputs.dxdydz, inputs.nbrCells, inputs.nbDim, "yee", inputs.interpOrder  };
   AllocSizeT allocSize{2*inputs.nbrCells[0],2*inputs.nbrCells[1],2*inputs.nbrCells[2]};
   Field field{allocSize, inputs.qty, "testField" }; // this is the important parameter here

   EXPECT_EQ(inputs.GEI[1], gl.ghostEndIndex     ( field, Direction::Y) );
}



TEST_P(GridLayoutIndexingTest, GhostEndIndexZ)
{
   GridLayoutParams inputs = GetParam(); // GetParam is from GTEST
   std::cout << inputs  << std::endl;
   GridLayout gl{ inputs.dxdydz, inputs.nbrCells, inputs.nbDim, "yee", inputs.interpOrder  };
   AllocSizeT allocSize{2*inputs.nbrCells[0],2*inputs.nbrCells[1],2*inputs.nbrCells[2]};
   Field field{allocSize, inputs.qty, "testField" }; // this is the important parameter here

   EXPECT_EQ(inputs.GEI[2], gl.ghostEndIndex     ( field, Direction::Z) );
}




INSTANTIATE_TEST_CASE_P(GridLayoutTest, GridLayoutIndexingTest,
                        testing::ValuesIn( getInputsFromFile() ) );



