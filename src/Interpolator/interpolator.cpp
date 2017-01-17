
#include "grid/gridlayoutdefs.h"

#include "Interpolator/interpolator.h"
#include "IndexesAndWeights/indexesandweightso1.h"
#include "IndexesAndWeights/indexesandweightso2.h"
#include "IndexesAndWeights/indexesandweightso3.h"
#include "IndexesAndWeights/indexesandweightso4.h"



Interpolator::Interpolator(uint32 order)
    : order_{order},
      impl_{ nullptr}
{

    switch( order )
    {
    case 1:
        impl_ = std::unique_ptr<IndexesAndWeightsO1>( new IndexesAndWeightsO1(order) ) ;
        break;
    case 2:
        impl_ = std::unique_ptr<IndexesAndWeightsO2>( new IndexesAndWeightsO2(order) ) ;
        break;
    case 3:
        impl_ = std::unique_ptr<IndexesAndWeightsO3>( new IndexesAndWeightsO3(order) ) ;
        break;
    case 4:
        impl_ = std::unique_ptr<IndexesAndWeightsO4>( new IndexesAndWeightsO4(order) ) ;
        break;
    }
}



std::tuple<std::vector<uint32>, std::vector<double>>
Interpolator::getIndexesAndWeights( Particle const & particle, \
                                    Direction dir, \
                                    QtyCentering centering ) const
{

    uint32 idir = static_cast<uint32>( dir ) ;

    // we compute the reduced coordinate
    // in the adequate direction
    double reducedCoord = particle.icell[idir] + \
            static_cast<double>(particle.delta[idir]) ;

    if(centering == QtyCentering::dual){
        reducedCoord += 0.5 ;
    }

    impl_->computeIndexes( reducedCoord ) ;
    impl_->computeWeights( reducedCoord ) ;

    std::vector<uint32> indexList  = impl_->indexList () ;
    std::vector<double> weightList = impl_->weightList() ;

    return std::make_tuple( indexList, weightList ) ;
}



std::tuple<std::vector<uint32>, std::vector<double>>
Interpolator::getIndexesAndWeights( Particle const & particle, Direction dir ) const
{

    uint32 idir = static_cast<uint32>( dir ) ;

    // we compute the reduced coordinate
    // in the adequate direction
    double reducedCoord = particle.icell[idir] + static_cast<double>(particle.delta[idir]) ;

    impl_->computeIndexes( reducedCoord ) ;
    impl_->computeWeights( reducedCoord ) ;

    std::vector<uint32> indexList  = impl_->indexList () ;
    std::vector<double> weightList = impl_->weightList() ;

    return std::make_tuple( indexList, weightList ) ;
}

