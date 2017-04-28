
#include <string>
#include <array>
#include <iostream>
#include <fstream>
#include <cmath>
#include <limits>


#include "Plasmas/particles.h"

#include "test_exactweights.h"

#include "IndexesAndWeights/indexesandweights.h"
#include "IndexesAndWeights/indexesandweightso1.h"
#include "IndexesAndWeights/indexesandweightso2.h"
#include "IndexesAndWeights/indexesandweightso3.h"
#include "IndexesAndWeights/indexesandweightso4.h"



uint32 ExactWeightsParams::testCaseNbr = 0 ;


::testing::AssertionResult AreWeightsEqual(
        const std::vector<double> & expected_vector,
        const std::vector<double> & actual_vector  ,
        double precision )
{
    ::testing::AssertionResult failure = ::testing::AssertionFailure();

    uint32 errorNbr = 0 ;

    if( expected_vector.size() != actual_vector.size() )
    {
        ::testing::AssertionResult sizeFailure = ::testing::AssertionFailure();

        sizeFailure << "Size failure" << "\n" ;
        sizeFailure << "expected_indexes.size() = " << expected_vector.size() << "\n" ;
        sizeFailure << "actual_indexes.size() = " << actual_vector.size() << "\n" ;

        return sizeFailure ;
    }

    for( uint32 ik=0 ; ik<actual_vector.size() ; ++ik)
    {
        if( fabs(expected_vector[ik] - actual_vector[ik]) > precision )
        {
            failure << "\n" ;
            failure << "expected[" << ik << "] = " << expected_vector[ik] ;
            failure << "    " ;
            failure << "actual  [" << ik << "] = " << actual_vector[ik] ;
            failure << "\n" ;
            ++errorNbr ;
        }
    }


    if( errorNbr > 0 )
    {
        // Error Summary
        failure << "\nTotal number of differences = " << errorNbr << "\n" ;

        return failure ;
    }

    return testing::AssertionSuccess() ;
}


class WeightsExact1DTest: public ::testing::TestWithParam<ExactWeightsParams>
{
public:
    ExactWeightsParams inputs;

    // std::numeric_limits<float>::epsilon()
    const double precision = std::numeric_limits<float>::epsilon() ;

    std::vector<double> expected_weights ;
    std::vector<double> actual_weights ;

    std::vector<uint32> indexes ;


    ~WeightsExact1DTest() = default ;

    void SetUp()
    {
        inputs = GetParam();

        std::string filename{"../IndexesAndWeights/weights_testCase"
                    + std::to_string(inputs.testId) + ".txt"};

        std::cout << filename << std::endl ;

        std::ifstream ifs2{filename};
        if (!ifs2 )
        {
            std::cout << "Could not open file : " << filename << std::endl ;
            exit(-1);
        }

        uint32 order = inputs.interpOrder ;

        expected_weights.assign( order+1, 0 ) ;
        for(uint32 ik=0 ; ik< (order+1) ; ik++)
        {
            ifs2 >> expected_weights[ik] ;
        }

        // get the node coordinate and the delta
        double icell = 0. ;
        double delta = std::modf(inputs.xpart/inputs.dx, &icell)  ;

        std::unique_ptr<IndexesAndWeights> impl  ;
        switch(order){
        case 1:
            impl = std::unique_ptr<IndexesAndWeightsO1>( new IndexesAndWeightsO1(order) ) ;
            break;
        case 2:
            impl = std::unique_ptr<IndexesAndWeightsO2>( new IndexesAndWeightsO2(order) ) ;
            break;
        case 3:
            impl = std::unique_ptr<IndexesAndWeightsO3>( new IndexesAndWeightsO3(order) ) ;
            break;
        case 4:
            impl = std::unique_ptr<IndexesAndWeightsO4>( new IndexesAndWeightsO4(order) ) ;
            break;
        }

        // test particle coordinate
        double Xreduced = icell + delta ;

        std::vector<uint32> indexes(order+1, 0) ;
        impl->computeIndexes(Xreduced, indexes) ;

        actual_weights.assign(order+1, 0.) ;
        impl->computeWeights(Xreduced, indexes, actual_weights) ;

    }


    void print(ExactWeightsParams const& inputs)
    {
        std::cout << "interpOrder : " << inputs.interpOrder
                  << " nbrX   : " << inputs.nbrX
                  << " dx = " << inputs.dx << "\n"
                  << " field = " << inputs.field
                  << " xmin = " << inputs.xmin
                  << " xpart = " << inputs.xpart << "\n"
                  << " testId = " << inputs.testId << "\n"
                  << " dxdydz[0] = " << inputs.dxdydz[0]
                  << " dxdydz[1] = " << inputs.dxdydz[0]
                  << " dxdydz[2] = " << inputs.dxdydz[0] << "\n"
                  << " nbrCells[0] = " << inputs.nbrCells[0]
                  << " nbrCells[1] = " << inputs.nbrCells[1]
                  << " nbrCells[2] = " << inputs.nbrCells[2] << "\n"
                  << " nbDim = " << inputs.nbDim << "\n"
                  << " lattice = " << inputs.lattice << "\n"
                  << std::endl ;
    }

};


/***********************************************************/
/* */
/*                                                         */
/***********************************************************/
TEST_P(WeightsExact1DTest, ExactWeights)
{

    EXPECT_TRUE( AreWeightsEqual(expected_weights, actual_weights, precision) );

}



INSTANTIATE_TEST_CASE_P(ExactWeightsTest, WeightsExact1DTest,
                        testing::ValuesIn( getExactWeightsParamsFromFile() ) );



