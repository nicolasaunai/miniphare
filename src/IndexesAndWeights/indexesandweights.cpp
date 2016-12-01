
#include <cmath>

#include "indexesandweights.h"




/**
 * @brief IndexesAndWeights::computeIndexes computes
 * the grid point indexes (indList), providing data
 * (for interpolation) or receiving data (for projection)
 *
 *
 */
void IndexesAndWeights::computeIndexes( double reducedCoord )
{
    // Compute primal integer mesh coordinates of the particle
    uint64 i_min = static_cast<uint64> \
            ( std::floor(reducedCoord - (static_cast<double> (order_)-1.)/2.) ) ;

    for( uint64 ik=0 ; ik<order_+1 ; ik++ )
    {
        indexList_[ik] = i_min + ik ;
    }

}

