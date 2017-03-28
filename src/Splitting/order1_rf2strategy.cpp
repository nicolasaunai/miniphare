

#include "Splitting/order1_rf2strategy.h"

#include "Distributions/distribgenerator.h"
#include "Distributions/distribstrategy.h"
#include "Distributions/uniformstrategy.h"



const uint32 interpOrder  = 1 ;
const uint32 refineFactor = 2 ;

/**
 * @brief Order1_RF2Strategy::Order1_RF2Strategy is a
 * specific case of OrderN_RF2Strategy.
 * This splitting strategy is exact, it works for 1st order and
 * a refinement factor of 2.
 *
 *
 * @param splitMethod
 */
Order1_RF2Strategy::Order1_RF2Strategy( const std::string & splitMethod )
    : SplittingStrategy( splitMethod,
                         (2*refineFactor -1) + (refineFactor -1)*(interpOrder-1) )
{

    child_icellx_[0] = -1 ;
    child_icellx_[1] =  0 ;
    child_icellx_[2] =  1 ;

    child_deltax_[0] = 0. ;
    child_deltax_[1] = 0. ;
    child_deltax_[2] = 0. ;

    child_weights_[0] = 1./2. ;
    child_weights_[1] = 1.    ;
    child_weights_[2] = 1./2. ;

    wtot_ = 2. ;
}





