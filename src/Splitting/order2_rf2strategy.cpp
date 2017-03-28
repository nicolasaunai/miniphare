

#include "Splitting/order2_rf2strategy.h"

#include "Distributions/distribgenerator.h"
#include "Distributions/distribstrategy.h"
#include "Distributions/uniformstrategy.h"



const uint32 interpOrder  = 2 ;
const uint32 refineFactor = 2 ;

/**
 * @brief Order2_RF2Strategy::Order2_RF2Strategy is a
 * specific case of OrderN_RF2Strategy.
 * This splitting strategy is exact, it works for 1st order and
 * a refinement factor of 2.
 *
 *
 * @param splitMethod
 */
Order2_RF2Strategy::Order2_RF2Strategy( const std::string & splitMethod )
    : SplittingStrategy( splitMethod,
                         (2*refineFactor -1) + (refineFactor -1)*(interpOrder-1) )
{

    child_icellx_[0] = -2 ;  // - 1.5*dxL1
    child_icellx_[1] = -1 ;  // - 0.5*dxL1
    child_icellx_[2] =  0 ;  // + 0.5*dxL1
    child_icellx_[3] =  1 ;  // + 1.5*dxL1

    child_deltax_[0] = 0.5 ;
    child_deltax_[1] = 0.5 ;
    child_deltax_[2] = 0.5 ;
    child_deltax_[3] = 0.5 ;

    child_weights_[0] = 1./4. ;
    child_weights_[1] = 3./4. ;
    child_weights_[2] = 3./4. ;
    child_weights_[3] = 1./4. ;

    wtot_ = 2. ;
}








