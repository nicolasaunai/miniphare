
#include <string>
#include <array>
#include <iostream>
#include <fstream>
#include <cmath>


#include "Plasmas/particles.h"

#include "test_indexes.h"

#include "IndexesAndWeights/indexesandweights.h"
#include "IndexesAndWeights/indexesandweightso1.h"
#include "IndexesAndWeights/indexesandweightso2.h"
#include "IndexesAndWeights/indexesandweightso3.h"
#include "IndexesAndWeights/indexesandweightso4.h"



uint32 IndexesParams::testCaseNbr = 0 ;


::testing::AssertionResult AreIndexListsEqual(
        const std::vector<uint32> & expected_indexes,
        const std::vector<uint32> & actual_indexes  )
{
    ::testing::AssertionResult failure = ::testing::AssertionFailure();

    uint32 errorNbr = 0 ;

    if( expected_indexes.size() != actual_indexes.size() )
    {
        ::testing::AssertionResult sizeFailure = ::testing::AssertionFailure();

        sizeFailure << "Size failure" << "\n" ;
        sizeFailure << "expected_indexes.size() = " << expected_indexes.size() << "\n" ;
        sizeFailure << "actual_indexes.size() = " << actual_indexes.size() << "\n" ;

        return sizeFailure ;
    }

    for( uint32 ik=0 ; ik<actual_indexes.size() ; ++ik)
    {
        if( actual_indexes[ik] != expected_indexes[ik])
        {
            failure << "\n" ;
            failure << "expected[" << ik << "] = " << expected_indexes[ik] ;
            failure << "    " ;
            failure << "actual  [" << ik << "] = " << actual_indexes[ik] ;
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

//        GridLayout layout{ inputs.dxdydz, inputs.nbrCells,
//                    inputs.nbDim, inputs.lattice, inputs.interpOrder  };


        std::string filename{"../IndexesAndWeights/indexes_testCase"
                    + std::to_string(inputs.testId) + ".txt"};

        std::cout << filename << std::endl ;

        std::ifstream ifs2{filename};
        if (!ifs2 )
        {
            std::cout << "Could not open file : " << filename << std::endl ;
            exit(-1);
        }

        expected_indexes.assign( inputs.interpOrder+1, 0 ) ;
        for(uint32 ik=0 ; ik< (inputs.interpOrder+1) ; ik++)
        {
            ifs2 >> expected_indexes[ik] ;
        }

        // get the node coordinate and the delta
        double icell = 0. ;
        double delta = std::modf(inputs.xpart/inputs.dx, &icell)  ;

        uint32 order = inputs.interpOrder ;

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

        actual_indexes.assign(order+1, 0) ;

        // We build the actual index List
        impl->computeIndexes(Xreduced, actual_indexes) ;
    }


    void print(IndexesParams const& inputs)
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
TEST_P(Indexes1DTest, IndexList)
{

    EXPECT_TRUE( AreIndexListsEqual(expected_indexes, actual_indexes ) );

}



INSTANTIATE_TEST_CASE_P(IndexesTest, Indexes1DTest,
                        testing::ValuesIn( getIndexesParamsFromFile() ) );



