
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



class GridLayoutAllocTest: public ::testing::TestWithParam<GridLayoutParams>
{
public:
    GridLayoutParams inputs;

    std::array<uint32,3> actual_allocSizes;
    std::array<uint32,3> actual_allocSizeDerived;


    void SetUp()
    {
        inputs = GetParam();

        print(inputs);

        GridLayout gl{ inputs.dxdydz, inputs.nbrCells, inputs.nbDim, "yee",
                    Point{0.,0.,0.}, inputs.interpOrder  };

        // Here the Field sizes for allocations are overestimated
        // We are not testing the alloc methods but only the methods devoted
        // to the determination of indexes
        AllocSizeT allocSize{2*inputs.nbrCells[0],2*inputs.nbrCells[1],2*inputs.nbrCells[2]};
        Field field{allocSize, inputs.qty, "testField" }; // this is the important parameter here

        actual_allocSizes[0] = gl.allocSize( field.hybridQty() ).nx_ ;
        actual_allocSizes[1] = gl.allocSize( field.hybridQty() ).ny_ ;
        actual_allocSizes[2] = gl.allocSize( field.hybridQty() ).nz_ ;

        actual_allocSizeDerived[0] = gl.allocSizeDerived(field.hybridQty(), Direction::X).nx_ ;
        actual_allocSizeDerived[1] = gl.allocSizeDerived(field.hybridQty(), Direction::Y).ny_ ;
        actual_allocSizeDerived[2] = gl.allocSizeDerived(field.hybridQty(), Direction::Z).nz_ ;
    }


    void print(GridLayoutParams const& inputs)
    {
        std::cout << " interpOrder : " << inputs.interpOrder
                  << " nbDims   : " << inputs.nbDim
                  << " (uint32) Hybridqty  : " << inputs.iqty << "\n"
                  << " nbrCells : " << inputs.nbrCells[0] << ", " << inputs.nbrCells[1] << ", " << inputs.nbrCells[2] << "\n"
                  << " dxdydz   : " << inputs.dxdydz[0] << ", " << inputs.dxdydz[1] << ", " << inputs.dxdydz[2] << "\n"
                  << " AllocSize x : " << inputs.allocSizes[0]
                  << " AllocSize y : " << inputs.allocSizes[1]
                  << " AllocSize z : " << inputs.allocSizes[2] << "\n"
                  << " Derived Size x : " << inputs.allocSizeDerived[0]
                  << " Derived Size y : " << inputs.allocSizeDerived[1]
                  << " Derived Size z : " << inputs.allocSizeDerived[2]
                  << std::endl ;
    }

};


/*-----------------------------------------------------------------------------
 *
 *
 *                          ALLOC SIZES
 *
 *
 *----------------------------------------------------------------------------*/

TEST_P(GridLayoutAllocTest, AllocSizeX)
{
    EXPECT_EQ(inputs.allocSizes[0], actual_allocSizes[0] );
}


TEST_P(GridLayoutAllocTest, AllocSizeY)
{
    EXPECT_EQ(inputs.allocSizes[1], actual_allocSizes[1] );
}


TEST_P(GridLayoutAllocTest, AllocSizeZ)
{
    EXPECT_EQ(inputs.allocSizes[2], actual_allocSizes[2] );
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
    EXPECT_EQ(inputs.allocSizeDerived[0], actual_allocSizeDerived[0] );
}


TEST_P(GridLayoutAllocTest, AllocSizeDerivedY)
{
    EXPECT_EQ(inputs.allocSizeDerived[1], actual_allocSizeDerived[1] );
}


TEST_P(GridLayoutAllocTest, AllocSizeDerivedZ)
{
    EXPECT_EQ(inputs.allocSizeDerived[2], actual_allocSizeDerived[2] );
}


INSTANTIATE_TEST_CASE_P(GridLayoutTest, GridLayoutAllocTest,
                        testing::ValuesIn( getAllocInputsFromFile() ) );



