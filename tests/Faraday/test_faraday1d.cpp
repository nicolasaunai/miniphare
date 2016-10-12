
#include <string>
#include <array>
#include <iostream>
#include <fstream>
#include <cmath>


#include "test_faraday.h"



MATCHER_P(FloatNear, tol, "Precision out of range")
{
    // we get the actual value
    double actualVal = std::get<0>(arg) ;

    // we get the expected value
    double expectedVal = std::get<1>(arg) ;

    return actualVal > expectedVal-tol && actualVal < expectedVal+tol ;
}


class Faraday1D: public ::testing::TestWithParam<FaradayParams>
{
public:
    FaradayParams inputs;

    double precision ;

    std::vector<double> expected_array ;
    std::vector<double> actual_array ;

    void SetUp()
    {
        inputs = GetParam();
        print(inputs) ;

        GridLayout gl{ inputs.dxdydz, inputs.nbrCells, inputs.nbDim, "yee", inputs.interpOrder  };

        // Here the Field sizes for allocations are overestimated
//        AllocSizeT allocSize{2*inputs.nbrCells[0],1,1};

        std::string testName = inputs.testName ;


        for( uint32 ifield=0 ; ifield<inputs.nbrOfFields ; ++ifield )
        {
            std::string fieldName = inputs.fieldNames[ifield] ;

            HybridQuantity qty = GetHybridQtyFromString( fieldName ) ;
            uint32 iqty = static_cast<uint32>(qty);

            for( uint32 itime=0 ; itime<inputs.nbrTimeSteps ; ++itime )
            {
                // faraday1D_test03_Bz_t10.txt
                std::string filename{"../Faraday/faraday1D_" + testName +
                            "_" + fieldName + "_t" + std::to_string(itime) + ".txt"};

                std::cout << filename << std::endl ;

                std::ifstream ifs1{filename};
                if (!ifs1 )
                {
                    std::cout << "Could not open file : " << filename << std::endl ;
                    exit(-1);
                }

//                uint32 iStart = gl.physicalStartIndex( qty, Direction::X ) ;
//                uint32 iEnd   = gl.physicalEndIndex( qty, Direction::X ) ;




            }
        }






//        uint32 iStart = inputs.field_iStart ;
//        uint32 iEnd   = inputs.field_iEnd   ;
//        for (uint32 ik=iStart ; ik<= iEnd ; ++ik)
//        {
//            infile >> inputs.fieldXCoords[ik] ;
//            infile >> inputs.fieldXValues[ik] ;
//            infile >> inputs.derivedFieldXCoords[ik] ;
//            infile >> inputs.derivedFieldXValues[ik] ;
//        }

//        // inputs.qty is the important parameter
//        Field operand{allocSize , inputs.qty, "operandField" };

//        for (uint32 ik=iStart ; ik<= iEnd ; ++ik)
//        {
//            double x = inputs.fieldXCoords[ik] ;

//            if(inputs.functionName == "linear")
//            {
//                operand(ik) = x ;
//            } else if(inputs.functionName == "square")
//            {
//                operand(ik) = x*x ;
//            } else if(inputs.functionName == "sin")
//            {
//                operand(ik) = sin(x) ;
//            } else if(inputs.functionName == "compo01")
//            {
//                operand(ik) = x*x + sin(x) ;
//            }
//        }

//        // inputs.qty is not the correct argument
//        // this should not be relevant for the test
//        Field derivative{allocSize , inputs.qty, "derivativeField" };

//        gl.deriv( operand, Direction::X, derivative ) ;

//        // this method has 2nd order precision
//        precision = pow(gl.dx(), 2.) ;

//        expected_array.assign(iEnd-iStart, 0.) ;
//        actual_array.assign(iEnd-iStart, 0.) ;
//        for( uint32 ix= iStart ; ix< iEnd ; ++ix )
//        {
//            expected_array[ix] = inputs.derivedFieldXValues[ix] ;
//            actual_array[ix]   = derivative(ix) ;
//        }
    }


    void print(FaradayParams const& inputs)
    {
        std::cout << "interpOrder : " << inputs.interpOrder
                  << " nbDims   : " << inputs.nbDim
                  << " nbr cell x = " << inputs.nbrCells[0] << ", nbr cell y = " \
                  << inputs.nbrCells[1] << ", nbr cell z = " << inputs.nbrCells[2]
                  << " dx = " << inputs.dxdydz[0] << ", dy = " \
                  << inputs.dxdydz[1] << ", dz = " << inputs.dxdydz[2]
                  << " dt = " << inputs.dt
                  << " tStart = " << inputs.tStart
                  << " tEnd = " << inputs.tEnd
                  << " testName = " << inputs.testName   ;

        std::cout << std::endl ;
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
TEST_P(Faraday1D, testFunction3)
{
    // This test is only for Bx field
    if( inputs.qtyName == "Bx" )
    {
        EXPECT_THAT( actual_array, \
                     ::testing::Pointwise(FloatNear(precision), expected_array) ) ;
    }

}



INSTANTIATE_TEST_CASE_P(FaradayTest, Faraday1D,
                        testing::ValuesIn( getFaraday1DInputsFromFile() ) );



