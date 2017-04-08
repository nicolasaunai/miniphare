
#include <memory>

#include "Plasmas/particles.h"

#include "grid/gridlayout.h"
#include "IndexesAndWeights/indexesandweights.h"
#include "IndexesAndWeights/indexesandweightso1.h"
#include "IndexesAndWeights/indexesandweightso2.h"
#include "IndexesAndWeights/indexesandweightso3.h"
#include "IndexesAndWeights/indexesandweightso4.h"

#include "Splitting/splittingstrategy.h"
#include "Splitting/splittingstrategyfactory.h"

#include "test_commons.h"
#include "test_splitting1part.h"



std::vector<Split1PartParams> getSplit1ParticleParamsFromFile() ;

std::unique_ptr<IndexesAndWeights>  createIndexesAndWeights( uint32 const & order ) ;

GridLayout buildRefinedLayout( Split1PartParams const & inputs ) ;




Split1PartParams split1ParticleInputs[] = {

    Split1PartParams({{0.1, 0., 0.}}, {{100, 1, 1}},    // dxdydz, nbrCellxyz
        1, "yee" , Point{0., 0., 0.},                // dim, layout, origin
        1, 2, "splitOrder1", 20)                     // order, RF, splitMethod, refNode

};




class Split1ParticleTest: public ::testing::TestWithParam<Split1PartParams>
{
public:
    std::vector<double> expected_weights ;

    std::vector<double> actual_weights ;


    Split1PartParams inputs;


    ~Split1ParticleTest() = default ;

    void SetUp()
    {
        inputs = GetParam();

        GridLayout coarseLayout{ inputs.dxdydz, inputs.nbrCells,
                                 inputs.nbDim , inputs.layout,
                                 inputs.origin, inputs.interpOrder} ;


        GridLayout refinedLayout{ buildRefinedLayout(inputs) } ;


        // we need a reference node
        uint32 refNode = inputs.referenceNode ;

        // we need an IndexesAndWeights object
        std::unique_ptr<IndexesAndWeights> indexAndWeights
                     { createIndexesAndWeights(inputs.interpOrder) } ;


        SplittingStrategyFactory factory(inputs.splitMethod,
                                         inputs.interpOrder,
                                         inputs.refineFactor ) ;

        std::unique_ptr<SplittingStrategy>
                strategy = factory.createSplittingStrategy() ;



        // the spatial step dx is discretized into
        // discreteNbr points
        uint32 discreteNbr = 10 ;

        // number of cells depending on the order
        // 1st order shape function covers 2*dx
        // 2nd order shape function covers 3*dx
        // 3rd order shape function covers 4*dx ...
        uint32 nbCells = discreteNbr*(inputs.interpOrder +1) ;
        uint32 nbPts = nbCells+1 ;

        float deltaMin = -0.5*static_cast<float>(inputs.interpOrder +1) ;

        expected_weights.assign( nbPts, 0. ) ;
        actual_weights.assign(nbPts, 0.) ;

        Particle normalizedMother ;

        /* ------------------------------------------------- */
        /* Expected weights are obtained by moving           */
        /* a mother particle (evolving on the coarse mesh)   */
        /* ------------------------------------------------- */

        // We now create the array of expected weights at a reference node.
        // To this sake we compute the contribution of a mother particle
        // at this reference node and next we move it slightly
        // delta += ik/discreteNbr
        //
        // Repeating this process nbPts times, we cover the variation
        // domain of the shape function
        //
        for( uint32 ik=0 ; ik<nbPts ; ik++ )
        {
            float delta = deltaMin + static_cast<float>(ik)/static_cast<float>(discreteNbr) ;

            // check the validity of delta (0 <= delta <= 1)
            // and do auto-correction
            float iCell = std::floor(delta) ;
            delta = delta - iCell ;

            // compute the shifted logical node
            // due to iCell displacement
            uint32 shiftedNode =  static_cast<uint32>(iCell) + refNode;

            // we now initialize a Particle at the reference node
            // Particle( weight, charge, icells, deltas, velocities )
            Particle mother( 10., 1., {{shiftedNode, 1, 1}},
                           {{delta, 0., 0.}}, {{0., 0., 0.}} );

            // normalized x coordinate
            double reducedX = static_cast<double>(mother.icell[0])
                    + static_cast<double>(mother.delta[0]) ;

            // Now, starts the algorithm
            std::vector<uint32> const & indexes = indexAndWeights->computeIndexes( reducedX ) ;
            std::vector<double> const & weights = indexAndWeights->computeWeights( reducedX ) ;

            std::vector<uint32>::const_iterator low ;
            low = std::lower_bound (indexes.begin(), indexes.end(), refNode) ;

            expected_weights[ik] = weights[*low] ;


            SplittingStrategy::normalizeMotherPosition1D(
                        coarseLayout, refinedLayout,
                        inputs.refineFactor,
                        mother, normalizedMother) ;

            std::vector<Particle> childParticles ;
            strategy->split1D( normalizedMother, childParticles ) ;







        }


    }

};




GridLayout buildRefinedLayout( Split1PartParams const & inputs )
{
    uint32 RF = inputs.refineFactor ;

    std::array<double, 3> dxdydz = inputs.dxdydz ;

    dxdydz[0] = inputs.dxdydz[0]/RF ;
    dxdydz[1] = inputs.dxdydz[1]/RF ;
    dxdydz[2] = inputs.dxdydz[2]/RF ;

    return GridLayout { dxdydz, inputs.nbrCells,
                inputs.nbDim , inputs.layout,
                Point{10*inputs.dxdydz[0], 0., 0.},
                inputs.interpOrder} ;
}




std::unique_ptr<IndexesAndWeights>  createIndexesAndWeights( uint32 const & order )
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

