

#include "Splitting/ordern_rf2strategy.h"

#include "multinomial.h"



const uint32 refineFactor = 2 ;

/**
 * @brief OrderN_RF2Strategy::OrderN_RF2Strategy
 * This splitting strategy is exact, it works for arbitrary order but
 * only when the refinement factor equals 2.
 * One mother Particle is split into:
 * 3, 4, 5, 6 children particles depending on order 1, 2, 3, 4
 *
 * See https://hephaistos.lpp.polytechnique.fr/redmine/projects/hyb-par/wiki/Bsplines
 * for algebraic details.
 * The polynomial decomposition requires to compute binomial coefficients.
 *
 * @param splitMethod
 * @param refineFactor
 * @param interpOrder
 */
OrderN_RF2Strategy::OrderN_RF2Strategy( const std::string & splitMethod,
                                        uint32 interpOrder )
    : SplittingStrategy( splitMethod,
                         (2*refineFactor -1) + (refineFactor -1)*(interpOrder-1) )
{

    float nbrCello2 = static_cast<float>( (nbpts_ - 1)/2. ) ;

    float delta = nbrCello2 - std::floor(nbrCello2);

    int32 icell = static_cast<int32>( std::floor(-nbrCello2) ) ;
    for( uint32 ik=0 ; ik<nbpts_ ; ++ik )
    {
        child_icellx_[ik] = icell ;
        ++icell ;

        child_deltax_[ik] = delta ;
    }

    for( uint32 ik=0 ; ik<nbpts_ ; ++ik )
    {
        child_weights_[ik] = static_cast<double>( binomial(interpOrder+1, ik) )/
                static_cast<double>( ipow(refineFactor, interpOrder) ) ;

        wtot_ += child_weights_[ik] ;
    }

}






