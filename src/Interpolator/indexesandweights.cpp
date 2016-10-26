
#include <cmath>

#include "indexesandweights.h"




IndexesAndWeights::~IndexesAndWeights() {}


void IndexesAndWeights::primalMeshCoord( const double & spart )
{
    // real coordinate expressed in primal mesh number
    // TODO : sminGlobal_ must be >= 0.
    meshCoord_ = ( spart - sminGlobal_ ) * ods_ ;

}



/**
 * @brief IndexesAndWeights::computeIndexes computes
 * the grid point indexes (indList), providing data
 * (for interpolation) or receiving data (for projection)
 *
 *
 */
void IndexesAndWeights::computeIndexes()
{
    // Compute primal integer mesh coordinates of the particle
    uint64 i_min = static_cast<uint64> \
            ( std::floor(meshCoord_ - (static_cast<double> (order_)-1.)/2.) ) ;

    for( uint64 ik=0 ; ik<order_+1 ; ik++ )
    {
        indexList_[ik] = i_min + ik ;
    }

}

