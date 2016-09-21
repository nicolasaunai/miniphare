
#include <string>
#include <array>
#include <iostream>
#include <fstream>


#include "gtest/gtest.h"


#include "grid/gridlayout.h"
#include "grid/gridlayoutimplfactory.h"
#include "grid/gridlayoutimplyee.h"
#include "types.h"
#include "utility.h"
#include "Field/field.h"







HybridQuantity GetHybridQty(uint iqty)
{
    switch( iqty)
    {
        case 0:
        return HybridQuantity::Bx;
        break;

        case 1:
        return HybridQuantity::By;
        break;

        case 2:
        return HybridQuantity::Bz;
        break;

        case 3:
        return HybridQuantity::Ex;
        break;

        case 4:
        return HybridQuantity::Ey;
        break;

        case 5:
        return HybridQuantity::Ez;
        break;

        case 6:
        return HybridQuantity::rho;
        break;

        case 7:
        return HybridQuantity::V;
        break;

        case 8:
        return HybridQuantity::P;
        break;

        default:
        return HybridQuantity::count;
    }
}



struct GridLayoutParams
{
    uint32 interpOrder;
    uint32 nbDim;
    HybridQuantity qty;
    std::array<uint32, 3> nbrCells;
    std::array<double, 3> dxdydz;

    std::array<uint32,3> PSI;
    std::array<uint32,3> PEI;
    std::array<uint32,3> GSI;
    std::array<uint32,3> GEI;
    uint32 iqty;


    friend std::ostream& operator<<(std::ostream& os, GridLayoutParams const& inputs)
    {
        os << "interpOrder : " << inputs.interpOrder
           << " nbDims   : " << inputs.nbDim
           << " qtyName  : " << static_cast<int>(inputs.qty)
           << " nbrCells : " << inputs.nbrCells[0] << ", " << inputs.nbrCells[1] << ", " << inputs.nbrCells[2]
           << " dxdydz   : " << inputs.dxdydz[0] << ", " << inputs.dxdydz[1] << ", " << inputs.dxdydz[2]
           << " PSI x : " << inputs.PSI[0] << " PEI x : " << inputs.PEI[0]
           << " GSI x : " << inputs.GSI[0] << " GEI x : " << inputs.GEI[0]
           << " PSI y : " << inputs.PSI[1] << " PEI y : " << inputs.PEI[1]
           << " GSI y : " << inputs.GSI[1] << " GEI y : " << inputs.GEI[1]
           << " PSI z : " << inputs.PSI[2] << " PEI z : " << inputs.PEI[2]
           << " GSI z : " << inputs.GSI[2] << " GEI z : " << inputs.GEI[2]
           << " " <<  inputs.iqty;


        return os;
    }

};






std::vector<GridLayoutParams> getInputsFromFile()
{

    std::ifstream ifs{"result.txt"};

    std::vector<GridLayoutParams> params(108);
    uint iqty;

    for (int i =0; i < 108; ++i)
    {

        ifs >> params[i].interpOrder >> params[i].nbDim >> iqty
        >> params[i].nbrCells[0] >>  params[i].nbrCells[1]  >>  params[i].nbrCells[2]
        >> params[i].dxdydz[0] >>  params[i].dxdydz[1]  >>  params[i].dxdydz[2]
        >> params[i].PSI[0] >> params[i].PEI[0] >> params[i].GSI[0] >> params[i].GEI[0]
        >> params[i].PSI[1] >> params[i].PEI[1] >> params[i].GSI[1] >> params[i].GEI[1]
        >> params[i].PSI[2] >> params[i].PEI[2] >> params[i].GSI[2] >> params[i].GEI[2];


        params[i].qty = GetHybridQty(iqty);
        params[i].iqty = iqty;
            //std::cout << params[i] << std::endl;

    }

    return params;
}





class GridLayoutIndexingTest: public ::testing::TestWithParam<GridLayoutParams>
{

};




TEST_P(GridLayoutIndexingTest, PhysicalStartIndexX)
{
    GridLayoutParams inputs = GetParam(); // GetParam is from GTEST
    std::cout << inputs  << std::endl;

    GridLayout gl{ inputs.dxdydz, inputs.nbrCells, inputs.nbDim, "yee", inputs.interpOrder  };

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

