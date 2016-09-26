
#include <string>
#include <array>
#include <iostream>

#include "types.h"

#include "grid/gridlayout.h"
#include "grid/gridlayoutimplfactory.h"
#include "grid/gridlayoutimpl.h"
#include "grid/gridlayoutimplyee.h"

#include "Field/field.h"

#include "test_gridlayout.h"



/*-----------------------------------------------------------------------------
 *
 *
 *                          ALLOC SIZES
 *
 *
 *----------------------------------------------------------------------------*/

TEST_P(GridLayoutAllocTest, AllocSizeX)
{
    GridLayoutParams inputs = GetParam(); // calls method getAllocInputsFromFile()
    std::cout << inputs  << std::endl;

    GridLayout gl{ inputs.dxdydz, inputs.nbrCells, inputs.nbDim, "yee", inputs.interpOrder  };

    // Here the Field sizes for allocations are overestimated
    // We are not testing the alloc methods but only the methods devoted
    // to the determination of indexes
    AllocSizeT allocSize{2*inputs.nbrCells[0],2*inputs.nbrCells[1],2*inputs.nbrCells[2]};
    Field field{allocSize, inputs.qty, "testField" }; // this is the important parameter here

    EXPECT_EQ(inputs.allocSizes[0], gl.allocSize( field.hybridQty() ).nx_ );
}




TEST_P(GridLayoutAllocTest, AllocSizeY)
{
   GridLayoutParams inputs = GetParam(); // calls method getAllocInputsFromFile()
   std::cout << inputs  << std::endl;
   GridLayout gl{ inputs.dxdydz, inputs.nbrCells, inputs.nbDim, "yee", inputs.interpOrder  };
   AllocSizeT allocSize{2*inputs.nbrCells[0],2*inputs.nbrCells[1],2*inputs.nbrCells[2]};
   Field field{allocSize, inputs.qty, "testField" }; // this is the important parameter here

   EXPECT_EQ(inputs.allocSizes[1], gl.allocSize( field.hybridQty() ).ny_ );
}




TEST_P(GridLayoutAllocTest, AllocSizeZ)
{
   GridLayoutParams inputs = GetParam(); // calls method getAllocInputsFromFile()
   std::cout << inputs  << std::endl;
   GridLayout gl{ inputs.dxdydz, inputs.nbrCells, inputs.nbDim, "yee", inputs.interpOrder  };
   AllocSizeT allocSize{2*inputs.nbrCells[0],2*inputs.nbrCells[1],2*inputs.nbrCells[2]};
   Field field{allocSize, inputs.qty, "testField" }; // this is the important parameter here

   EXPECT_EQ(inputs.allocSizes[2], gl.allocSize( field.hybridQty() ).nz_ );
}


/*-----------------------------------------------------------------------------
 *
 *
 *                          ALLOC SIZES  FOR  DERIVATIVES
 *
 *
 *----------------------------------------------------------------------------*/

TEST_P(GridLayoutAllocTest, AllocSizeDerivedX)
{
    GridLayoutParams inputs = GetParam(); // calls method getAllocInputsFromFile()
    std::cout << inputs  << std::endl;

    GridLayout gl{ inputs.dxdydz, inputs.nbrCells, inputs.nbDim, "yee", inputs.interpOrder  };

    // Here the Field sizes for allocations are overestimated
    // We are not testing the alloc methods but only the methods devoted
    // to the determination of indexes
    AllocSizeT allocSize{2*inputs.nbrCells[0],2*inputs.nbrCells[1],2*inputs.nbrCells[2]};
    Field field{allocSize, inputs.qty, "testField" }; // this is the important parameter here

    EXPECT_EQ(inputs.allocSizeDerived[0],
              gl.allocSizeDerived(field.hybridQty(), Direction::X).nx_ );
}




TEST_P(GridLayoutAllocTest, AllocSizeDerivedY)
{
   GridLayoutParams inputs = GetParam(); // calls method getAllocInputsFromFile()
   std::cout << inputs  << std::endl;
   GridLayout gl{ inputs.dxdydz, inputs.nbrCells, inputs.nbDim, "yee", inputs.interpOrder  };
   AllocSizeT allocSize{2*inputs.nbrCells[0],2*inputs.nbrCells[1],2*inputs.nbrCells[2]};
   Field field{allocSize, inputs.qty, "testField" }; // this is the important parameter here

   EXPECT_EQ(inputs.allocSizeDerived[1],
             gl.allocSizeDerived(field.hybridQty(), Direction::Y).ny_ );
}




TEST_P(GridLayoutAllocTest, AllocSizeDerivedZ)
{
   GridLayoutParams inputs = GetParam(); // calls method getAllocInputsFromFile()
   std::cout << inputs  << std::endl;
   GridLayout gl{ inputs.dxdydz, inputs.nbrCells, inputs.nbDim, "yee", inputs.interpOrder  };
   AllocSizeT allocSize{2*inputs.nbrCells[0],2*inputs.nbrCells[1],2*inputs.nbrCells[2]};
   Field field{allocSize, inputs.qty, "testField" }; // this is the important parameter here

   EXPECT_EQ(inputs.allocSizeDerived[2],
             gl.allocSizeDerived(field.hybridQty(), Direction::Z).nz_ );
}





INSTANTIATE_TEST_CASE_P(GridLayoutTest, GridLayoutAllocTest,
                        testing::ValuesIn( getAllocInputsFromFile() ) );



