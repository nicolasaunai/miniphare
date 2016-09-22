
#include <string>
#include <array>
#include <iostream>
#include <fstream>


#include "test_gridlayout.h"


HybridQuantity GetHybridQty(uint32 iqty)
{
    HybridQuantity quantity = HybridQuantity::count ;

    std::array<HybridQuantity, static_cast<uint32>(HybridQuantity::count) >  hybridQtyTable ;

    hybridQtyTable[0] = HybridQuantity::Bx  ;
    hybridQtyTable[1] = HybridQuantity::By  ;
    hybridQtyTable[2] = HybridQuantity::Bz  ;
    hybridQtyTable[3] = HybridQuantity::Ex  ;
    hybridQtyTable[4] = HybridQuantity::Ey  ;
    hybridQtyTable[5] = HybridQuantity::Ez  ;
    hybridQtyTable[6] = HybridQuantity::rho ;
    hybridQtyTable[7] = HybridQuantity::V   ;
    hybridQtyTable[8] = HybridQuantity::P   ;

    if( iqty < static_cast<uint32>(HybridQuantity::count) )
    {
        quantity = hybridQtyTable[iqty] ;
    } else
    {
        quantity = HybridQuantity::count ;
    }

    return quantity ;
}



std::vector<GridLayoutParams> getInputsFromFile()
{

    std::ifstream ifs{"result.txt"};

    uint32 orderMax = 4 ;
    uint32 dimMax = 3 ;

    uint32 nbrTestCases = orderMax * dimMax * static_cast<uint32>(HybridQuantity::count) ;

    std::vector<GridLayoutParams> params(nbrTestCases);
    uint32 iqty;

    for (uint32 i=0 ; i < nbrTestCases ; ++i)
    {
        ifs >> params[i].interpOrder >> params[i].nbDim >> iqty
        >> params[i].nbrCells[0] >>  params[i].nbrCells[1]  >>  params[i].nbrCells[2]
        >> params[i].dxdydz[0] >>  params[i].dxdydz[1]  >>  params[i].dxdydz[2]
        >> params[i].PSI[0] >> params[i].PEI[0] >> params[i].GSI[0] >> params[i].GEI[0]
        >> params[i].PSI[1] >> params[i].PEI[1] >> params[i].GSI[1] >> params[i].GEI[1]
        >> params[i].PSI[2] >> params[i].PEI[2] >> params[i].GSI[2] >> params[i].GEI[2];


        params[i].qty = GetHybridQty(iqty);
        params[i].iqty = iqty;
    }

    return params;
}



TEST_P(GridLayoutIndexingTest, PhysicalStartIndexX)
{
    GridLayoutParams inputs = GetParam(); // GetParam is from GTEST
    std::cout << inputs  << std::endl;

    GridLayout gl{ inputs.dxdydz, inputs.nbrCells, inputs.nbDim, "yee", inputs.interpOrder  };

    // Here the Field sizes for allocations are overestimated
    // We are not testing the alloc methods but only the methods devoted
    // to the determination of indexes
    Field field{2*inputs.nbrCells[0],2*inputs.nbrCells[1],2*inputs.nbrCells[2],
                inputs.qty,            // this is the important parameter here
                "testField" };

    EXPECT_EQ(inputs.PSI[0], gl.physicalStartIndex( field, Direction::X) );
    EXPECT_EQ(inputs.PEI[0], gl.physicalEndIndex  ( field, Direction::X) );
    EXPECT_EQ(inputs.GSI[0], gl.ghostStartIndex   ( field, Direction::X) );
    EXPECT_EQ(inputs.GEI[0], gl.ghostEndIndex     ( field, Direction::X) );

    EXPECT_EQ(inputs.PSI[1], gl.physicalStartIndex( field, Direction::Y) );
    EXPECT_EQ(inputs.PEI[1], gl.physicalEndIndex  ( field, Direction::Y) );
    EXPECT_EQ(inputs.GSI[1], gl.ghostStartIndex   ( field, Direction::Y) );
    EXPECT_EQ(inputs.GEI[1], gl.ghostEndIndex     ( field, Direction::Y) );

    EXPECT_EQ(inputs.PSI[2], gl.physicalStartIndex( field, Direction::Z) );
    EXPECT_EQ(inputs.PEI[2], gl.physicalEndIndex  ( field, Direction::Z) );
    EXPECT_EQ(inputs.GSI[2], gl.ghostStartIndex   ( field, Direction::Z) );
    EXPECT_EQ(inputs.GEI[2], gl.ghostEndIndex     ( field, Direction::Z) );
}





INSTANTIATE_TEST_CASE_P(GridLayoutTest, GridLayoutIndexingTest,
                        testing::ValuesIn( getInputsFromFile() ) );





int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

