
#include "grid/gridlayoutdefs.h"

#include "Projector/projector.h"


std::tuple<std::vector<uint32>, std::vector<double>>
Projector::getIndexesAndWeights( Particle const & particle, Direction dir )
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


