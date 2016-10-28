#ifndef INDEXESANDWEIGHTS_H
#define INDEXESANDWEIGHTS_H


#include <vector>

#include "types.h"


/**
 * @brief The IndexesAndWeights class handles computation of
 * the list of indexes and the list of weights necessary to
 * deposit (on the grid) or interpolate (to a particle) a
 * physical quantity.
 *
 * Note that a 3D shape function is obtained by two successive
 * tensorial products of a 1D shape function :
 * S(x, y, z) = S(x) * S(y) * S(z)
 *
 * Therefore, we restrict this class to a 1D treatment to
 * account for a 1D shape function
 *
 * It receives 3 inputs:
 * - the interpolation order
 * - the inverse of the spatial step
 * - a global minimum in a direction
 *
 * It produces 2 outputs (used by Interpolator or Projector):
 * - the grid point indexes (indList)
 * - the weights (or ponderations) associated to each point of indList
 *
 * The indexes are computed by this class
 * with method computeIndexes (factorized)
 *
 * The weights are computed by subclasses (method computeWeights)
 * devoted to order 1, 2, 3 and 4.
 *
 */
class IndexesAndWeights
{
private:
      uint32  order_ ;

      double ods_ ;
      double sminGlobal_ ;

protected:
      std::vector<uint64> indexList_ ;
      std::vector<double> weightList_ ;

public:
      IndexesAndWeights( uint32 order, double ods, double sminGlobal )
          : order_{order}, ods_{ods}, sminGlobal_{sminGlobal}
      {
          indexList_.assign( order_+1, 0 ) ;
          weightList_.assign( order_+1, 0. ) ;
      }

      IndexesAndWeights( const IndexesAndWeights & ) = default ;
      IndexesAndWeights & operator=(const IndexesAndWeights &) = default ;

      virtual ~IndexesAndWeights() ;

      // spart is a 1D coordinate in physical unit
      double reducedCoord( double spart ) ;

      void computeIndexes( double reducedCoord ) ;

      // this method depends on the interpolation order
      virtual void computeWeights( double reducedCoord ) = 0 ;

};

#endif // INDEXESANDWEIGHTS_H
