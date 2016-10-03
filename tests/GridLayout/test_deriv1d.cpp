
#include <string>
#include <array>
#include <iostream>
#include <fstream>
#include <cmath>


#include "test_gridlayout.h"




void testDerivedField1D( GridLayoutParams & inputs,
                       const GridLayout & gl     )
{

    // Here the Field sizes for allocations are overestimated
    AllocSizeT allocSize{2*inputs.nbrCells[0],1,1};

    // inputs.qty is the important parameter
    Field field{allocSize , inputs.qty, "testField" };

//    uint32 iy = gl.indexAtMin( QtyCentering::primal, Direction::Y ) ;
//    uint32 iz = gl.indexAtMin( QtyCentering::primal, Direction::Z ) ;

//    uint32 order = inputs.interpOrder ;
//    uint32 dim = inputs.nbDim ;

    std::string qtyName = inputs.qtyName;

    std::string fctName = inputs.functionName;

    std::string filename{"../GridLayout/deriv1D_" + qtyName +
                  "_" + fctName + ".txt"};

    std::cout << filename << std::endl ;

    std::ifstream infile{filename};
    if (!infile )
    {
        std::cout << "Could not open file : " << filename << std::endl ;
        exit(-1);
    }

    uint32 iStart = inputs.field_iStart ;
    uint32 iEnd   = inputs.field_iEnd   ;
    for (uint32 ik=iStart ; ik<= iEnd ; ++ik)
    {
        infile >> inputs.fieldXCoords[ik] ;
        infile >> inputs.fieldXValues[ik] ;
        infile >> inputs.derivedFieldXCoords[ik] ;
        infile >> inputs.derivedFieldXValues[ik] ;
    }

    for (uint32 ik=iStart ; ik<= iEnd ; ++ik)
    {
        double x = inputs.fieldXCoords[ik] ;

        if(inputs.functionName == "linear")
        {
            field(ik) = x ;
        } else if(inputs.functionName == "square")
        {
            field(ik) = x*x ;
        } else if(inputs.functionName == "sin")
        {
            field(ik) = sin(x) ;
        } else if(inputs.functionName == "compo01")
        {
            field(ik) = x*x + sin(x) ;
        }
    }

    for( uint32 ix= iStart ; ix<= iEnd ; ix++ )
    {
//        Point ptest = gl.fieldNodeCoordinates(field, inputs.origin, ix, iy, iz) ;

        ASSERT_DOUBLE_EQ( inputs.fieldXValues[ix], field(ix) ) ;
    }


}



TEST_P(GridLayoutDeriv1D, DerivedFieldBx)
{
    GridLayoutParams inputs = GetParam();
    std::cout << inputs  << std::endl;

    GridLayout gl{ inputs.dxdydz, inputs.nbrCells, inputs.nbDim, "yee", inputs.interpOrder  };

    // This test is only for Bx field
    if( inputs.qtyName == "Bx" )
    {
        testDerivedField1D( inputs, gl ) ;
    }

}



INSTANTIATE_TEST_CASE_P(GridLayoutTest, GridLayoutDeriv1D,
                        testing::ValuesIn( getDerivInputsFromFile() ) );



