
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

    // inputs.qty is the important parameter
    Field operand{allocSize , inputs.qty, "operandField" };

    for (uint32 ik=iStart ; ik<= iEnd ; ++ik)
    {
        double x = inputs.fieldXCoords[ik] ;

        if(inputs.functionName == "linear")
        {
            operand(ik) = x ;
        } else if(inputs.functionName == "square")
        {
            operand(ik) = x*x ;
        } else if(inputs.functionName == "sin")
        {
            operand(ik) = sin(x) ;
        } else if(inputs.functionName == "compo01")
        {
            operand(ik) = x*x + sin(x) ;
        }
    }

//    for( uint32 ix= iStart ; ix<= iEnd ; ++ix )
//    {
//        // float precision is enough for the test
//        ASSERT_FLOAT_EQ( static_cast<float>(inputs.fieldXValues[ix]),
//                         static_cast<float>(operand(ix)              ) ) ;
//    }

    // inputs.qty is not the correct argument
    // this should not be relevant for the test
    Field derivative{allocSize , inputs.qty, "derivativeField" };

    gl.deriv( operand, Direction::X, derivative ) ;

    // this method has 2nd order precision
    double precision = pow(gl.dx(), 2.) ;

    for( uint32 ix= iStart ; ix< iEnd ; ++ix )
    {
        // precision must be monitored for this test
        ASSERT_NEAR( inputs.derivedFieldXValues[ix], derivative(ix), precision ) ;
    }


}


/***********************************************************/
/* Testing Bx is enough                                    */
/* Our only goal here is to test the derivative operator   */
/*                                                         */
/* The numerically computed derivative is accurate to the  */
/* 2nd order                                               */
/*                                                         */
/***********************************************************/
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



