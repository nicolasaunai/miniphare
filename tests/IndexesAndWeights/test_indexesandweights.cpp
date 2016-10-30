
#include <string>
#include <array>
#include <iostream>
#include <fstream>
#include <cmath>

#include "test_indexesandweights.h"

#include "Faraday/faraday.h"
#include "Faraday/faradayimpl1d.h"
#include "Faraday/faradayfactory.h"




::testing::AssertionResult AreIndexListsEqual(
        const std::vector<uint32> & expected_indexes,
        const std::vector<uint32> & actual_indexes  )
{
    ::testing::AssertionResult failure = ::testing::AssertionFailure();

    uint32 errorNbr = 1 ;

    if( errorNbr > 0 )
    {
        // Error Summary
        failure << "\nTotal number of differences = " << errorNbr << "\n" ;

        return failure ;
    }

    return testing::AssertionSuccess() ;
}


class Indexes1DTest: public ::testing::TestWithParam<IndexesParams>
{
public:
    IndexesParams inputs;

    double precision ;

    std::vector<uint32> expected_indexes ;
    std::vector<uint32> actual_indexes ;


    ~Indexes1DTest() = default ;

    void SetUp()
    {
        inputs = GetParam();
        print(inputs) ;

        GridLayout layout{ inputs.dxdydz, inputs.nbrCells, inputs.nbDim, inputs.lattice, inputs.interpOrder  };






    }


    void print(IndexesParams const& inputs)
    {
        std::cout << "interpOrder : " << inputs.interpOrder
                  << " nbrX   : " << inputs.nbrX
                  << " dx = " << inputs.dx
                  << " field = " << inputs.field
                  << " xmin = " << inputs.xmin
                  << " xpart = " << inputs.xpart
                  << " dxdydz[0] = " << inputs.dxdydz[0]
                  << " dxdydz[1] = " << inputs.dxdydz[0]
                  << " dxdydz[2] = " << inputs.dxdydz[0]
                  << " nbrCells[0] = " << inputs.nbrCells[0]
                  << " nbrCells[1] = " << inputs.nbrCells[1]
                  << " nbrCells[2] = " << inputs.nbrCells[2]
                  << " nbDim = " << inputs.nbDim
                  << " lattice = " << inputs.lattice
                  << std::endl ;
    }

};


/***********************************************************/
/* */
/*                                                         */
/***********************************************************/
TEST_P(Indexes1DTest, IndexList)
{

    EXPECT_TRUE( AreIndexListsEqual(expected_indexes, actual_indexes ) );

}



INSTANTIATE_TEST_CASE_P(IndexesTest, Indexes1DTest,
                        testing::ValuesIn( getIndexesParamsFromFile() ) );



