
#include <string>
#include <array>
#include <iostream>

#include <cmath>
#include <limits>

#include "Plasmas/particles.h"

#include "test_weights.h"

#include "IndexesAndWeights/indexesandweights.h"
#include "IndexesAndWeights/indexesandweightso1.h"
#include "IndexesAndWeights/indexesandweightso2.h"
#include "IndexesAndWeights/indexesandweightso3.h"
#include "IndexesAndWeights/indexesandweightso4.h"



class Weights1DTest: public ::testing::TestWithParam<WeightParams>
{
public:
    WeightParams inputs;

    std::vector<uint32> indexTable ;
    std::vector<double> weightTable ;

    double weightSum = 0. ;

    // 2.e-7 is greater than
    // std::numeric_limits<float>::epsilon()
    const double precision = std::numeric_limits<float>::epsilon() ;

    ~Weights1DTest() = default ;

    void SetUp()
    {
        inputs = GetParam();
        print(inputs) ;

//        GridLayout layout{ inputs.dxdydz, inputs.nbrCells, \
//                    inputs.nbDim, inputs.lattice, inputs.interpOrder  };

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
        double reduced = icell + delta ;

        // We build the index List
        impl->computeIndexes(reduced) ;

        // We build the weight list
        impl->computeWeights(reduced) ;

        // We copy the index table
        indexTable = impl->indexList() ;

        // We copy the weight table
        weightTable = impl->weightList() ;

        weightSum = 0. ;
        for(uint32 ik=0 ; ik<weightTable.size() ; ik++)
        {
            weightSum += weightTable[ik] ;
        }

        printIndexTable( indexTable ) ;

        printWeightTable( weightTable ) ;

    }


    void print( WeightParams const& inputs )
    {
        std::cout << "interpOrder : " << inputs.interpOrder
                  << " nbrX   : " << inputs.nbrX
                  << " dx = " << inputs.dx << "\n"
//                  << " field = " << inputs.field
                  << " xmin = " << inputs.xmin
                  << " xpart = " << inputs.xpart << "\n"
//                  << " dxdydz[0] = " << inputs.dxdydz[0]
//                  << " dxdydz[1] = " << inputs.dxdydz[1]
//                  << " dxdydz[2] = " << inputs.dxdydz[2] << "\n"
//                  << " nbrCells[0] = " << inputs.nbrCells[0]
//                  << " nbrCells[1] = " << inputs.nbrCells[1]
//                  << " nbrCells[2] = " << inputs.nbrCells[2] << "\n"
//                  << " nbDim = " << inputs.nbDim << "\n"
//                  << " lattice = " << inputs.lattice << "\n"
                  << std::endl ;
    }

    void printIndexTable( std::vector<uint32> const & indexTable )
    {
        std::cout << "- indexTable -" << std::endl ;
        for( uint32 ik=0 ; ik<indexTable.size() ; ++ik)
        {
            std::cout << " [" << ik << "] = " << indexTable[ik] << "," ;
        }
        std::cout << std::endl ;
    }

    void printWeightTable( std::vector<double> const & weightTable )
    {
        std::cout << "- weightTable -" << std::endl ;
        for( uint32 ik=0 ; ik<weightTable.size() ; ++ik)
        {
            std::cout << " [" << ik << "] = " << weightTable[ik] << "," ;
        }
        std::cout << std::endl ;
    }


};


/***********************************************************/
/* */
/*                                                         */
/***********************************************************/
TEST_P(Weights1DTest, SumEqualsOne)
{

    double expected = 1. ;
    double actual = weightSum ;

    EXPECT_NEAR(expected, actual, precision) ;
}

TEST_P(Weights1DTest, Positivity)
{
    using namespace testing;

    EXPECT_THAT(weightTable, Each( AllOf( Ge(0.), Lt(1.)) ) );
}


// WeightParams(uint32 order, uint32 nbrCellX, double dx,
//              std::string field, double xmin, double xpart)
static WeightParams weightParamsInputs[] = {

    WeightParams( 1, 40, 0.1, "rho", 0., 1.01 ),
    WeightParams( 1, 40, 0.1, "rho", 0., 1.02 ),
    WeightParams( 1, 40, 0.1, "rho", 0., 1.03 ),
    WeightParams( 1, 40, 0.1, "rho", 0., 1.04 ),
    WeightParams( 1, 40, 0.1, "rho", 0., 1.05 ),
    WeightParams( 1, 40, 0.1, "rho", 0., 1.06 ),
    WeightParams( 1, 40, 0.1, "rho", 0., 1.07 ),
    WeightParams( 1, 40, 0.1, "rho", 0., 1.08 ),
    WeightParams( 1, 40, 0.1, "rho", 0., 1.09 ),
    WeightParams( 2, 40, 0.1, "rho", 0., 1.01 ),
    WeightParams( 2, 40, 0.1, "rho", 0., 1.02 ),
    WeightParams( 2, 40, 0.1, "rho", 0., 1.03 ),
    WeightParams( 2, 40, 0.1, "rho", 0., 1.04 ),
    WeightParams( 2, 40, 0.1, "rho", 0., 1.05 ),
    WeightParams( 2, 40, 0.1, "rho", 0., 1.06 ),
    WeightParams( 2, 40, 0.1, "rho", 0., 1.07 ),
    WeightParams( 2, 40, 0.1, "rho", 0., 1.08 ),
    WeightParams( 2, 40, 0.1, "rho", 0., 1.09 ),
    WeightParams( 3, 40, 0.1, "rho", 0., 1.01 ),
    WeightParams( 3, 40, 0.1, "rho", 0., 1.02 ),
    WeightParams( 3, 40, 0.1, "rho", 0., 1.03 ),
    WeightParams( 3, 40, 0.1, "rho", 0., 1.04 ),
    WeightParams( 3, 40, 0.1, "rho", 0., 1.05 ),
    WeightParams( 3, 40, 0.1, "rho", 0., 1.06 ),
    WeightParams( 3, 40, 0.1, "rho", 0., 1.07 ),
    WeightParams( 3, 40, 0.1, "rho", 0., 1.08 ),
    WeightParams( 3, 40, 0.1, "rho", 0., 1.09 ),
    WeightParams( 4, 40, 0.1, "rho", 0., 1.01 ),
    WeightParams( 4, 40, 0.1, "rho", 0., 1.02 ),
    WeightParams( 4, 40, 0.1, "rho", 0., 1.03 ),
    WeightParams( 4, 40, 0.1, "rho", 0., 1.04 ),
    WeightParams( 4, 40, 0.1, "rho", 0., 1.05 ),
    WeightParams( 4, 40, 0.1, "rho", 0., 1.06 ),
    WeightParams( 4, 40, 0.1, "rho", 0., 1.07 ),
    WeightParams( 4, 40, 0.1, "rho", 0., 1.08 ),
    WeightParams( 4, 40, 0.1, "rho", 0., 1.09 )

};


INSTANTIATE_TEST_CASE_P(WeightsTest, Weights1DTest,
                        testing::ValuesIn( weightParamsInputs ) );



