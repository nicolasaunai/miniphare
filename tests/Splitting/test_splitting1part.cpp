
#include <memory>

#include "Plasmas/particles.h"

#include "grid/gridlayout.h"
#include "IndexesAndWeights/indexesandweights.h"
#include "IndexesAndWeights/indexesandweightso1.h"
#include "IndexesAndWeights/indexesandweightso2.h"
#include "IndexesAndWeights/indexesandweightso3.h"
#include "IndexesAndWeights/indexesandweightso4.h"

#include "test_commons.h"
#include "test_splitting1part.h"



std::vector<Split1PartParams> getSplit1ParticleParamsFromFile() ;

std::unique_ptr<IndexesAndWeights>  getIndexesAndWeights( uint32 const & order ) ;



Split1PartParams split1ParticleInputs[] = {
    //                dxdydz            nbrCellx y z    layout  origin             order   refNode
    Split1PartParams({{0.1, 0., 0.}}, {{100, 1, 1}}, 1, "yee" , Point{0., 0., 0.}, 1     , 10)

};




class Split1ParticleTest: public ::testing::TestWithParam<Split1PartParams>
{
public:
    Split1PartParams inputs;


    ~Split1ParticleTest() = default ;

    void SetUp()
    {
        inputs = GetParam();

        // we need a layout
        GridLayout layout{ inputs.dxdydz, inputs.nbrCells,
                    inputs.nbDim, inputs.layout,
                    inputs.origin, inputs.interpOrder} ;

        // we need a reference node
        uint32 refNode = inputs.referenceNode ;

        // we need an IndexesAndWeights object
        std::unique_ptr<IndexesAndWeights> indexAndWeights { getIndexesAndWeights(inputs.interpOrder) } ;

        // we now initialize a Particle at the reference node
        Particle partic(
                    10., 1., {{refNode, 1, 1}},
                    {{0., 0., 0.}}, {{0., 0., 0.}} );

        // Now, starts the algorithm


    }

};




std::unique_ptr<IndexesAndWeights>  getIndexesAndWeights( uint32 const & order )
{
    std::unique_ptr<IndexesAndWeights> indexAndWeight_ptr {nullptr} ;

    switch (order)
    {
    case 1:
        indexAndWeight_ptr = std::unique_ptr<IndexesAndWeightsO1>( new IndexesAndWeightsO1(order) ) ;
        break;
    case 2:
        indexAndWeight_ptr = std::unique_ptr<IndexesAndWeightsO1>( new IndexesAndWeightsO1(order) ) ;
        break;
    case 3:
        indexAndWeight_ptr = std::unique_ptr<IndexesAndWeightsO1>( new IndexesAndWeightsO1(order) ) ;
        break;
    case 4:
        indexAndWeight_ptr = std::unique_ptr<IndexesAndWeightsO1>( new IndexesAndWeightsO1(order) ) ;
        break;
    }

    return indexAndWeight_ptr ;
}




TEST_P(Split1ParticleTest, shapes)
{


}



INSTANTIATE_TEST_CASE_P(Split1ParticleTest, Split1ParticleTest,
                        testing::ValuesIn( split1ParticleInputs ) );

