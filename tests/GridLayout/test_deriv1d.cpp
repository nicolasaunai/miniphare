
#include <string>
#include <array>
#include <iostream>
#include <fstream>
#include <cmath>


#include "test_gridlayout.h"




class GridLayoutDeriv1D: public ::testing::TestWithParam<GridLayoutParams>
{
public:
    GridLayoutParams inputs;

    double precision ;

    std::vector<double> expected_array ;
    std::vector<double> actual_array ;

    void SetUp()
    {
        inputs = GetParam();
        print(inputs) ;

        GridLayout gl{ inputs.dxdydz, inputs.nbrCells, inputs.nbDim, "yee", inputs.interpOrder  };

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

        // inputs.qty is not the correct argument
        // this should not be relevant for the test
        Field derivative{allocSize , inputs.qty, "derivativeField" };

        gl.deriv( operand, Direction::X, derivative ) ;

        // this method has 2nd order precision
        precision = pow(gl.dx(), 2.) ;

        expected_array.assign(iEnd-iStart, 0.) ;
        actual_array.assign(iEnd-iStart, 0.) ;
        for( uint32 ix= iStart ; ix< iEnd ; ++ix )
        {
            expected_array[ix] = inputs.derivedFieldXValues[ix] ;
            actual_array[ix]   = derivative(ix) ;
        }
    }


    void print(GridLayoutParams const& inputs)
    {
        std::cout << "interpOrder : " << inputs.interpOrder
                  << " nbDims   : " << inputs.nbDim
                  << " qtyName  : " << static_cast<int>(inputs.qty)
                  << " nbrCells : " << inputs.nbrCells[0] << ", " \
                  << inputs.nbrCells[1] << ", " << inputs.nbrCells[2]
                  << " dxdydz   : " << inputs.dxdydz[0] << ", " \
                  << inputs.dxdydz[1] << ", " << inputs.dxdydz[2]
                  << " " <<  inputs.iqty;
    }

};


/***********************************************************/
/* Testing Bx is enough                                    */
/* Our only goal here is to test the derivative operator   */
/*                                                         */
/* The numerically computed derivative is accurate to the  */
/* 2nd order                                               */
/*                                                         */
/***********************************************************/
TEST_P(GridLayoutDeriv1D, DerivedField)
{
        EXPECT_THAT( actual_array, \
                     ::testing::Pointwise(DoubleNear(precision), expected_array) ) ;
}



INSTANTIATE_TEST_CASE_P(GridLayoutTest, GridLayoutDeriv1D,
                        testing::ValuesIn( getDerivInputsFromFile() ) );



