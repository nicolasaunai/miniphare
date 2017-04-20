
#include "Splitting/order1_rfnstrategy.h"





/**
 * @brief Order1_RFnStrategy::Order1_RFnStrategy
 * This splitting strategy is exact, it works for arbitrary refinement factor
 * but only for 1st order shape function.
 * One mother Particle is split into:
 * 3, 5, 7 children particles when the refinement factor
 * equals 2, 3, 4
 *
 * See: miniphare/doc/html/md_markdown_exactSplitting_BSplines.html
 *
 * @param splitMethod
 * @param refineFactor
 * @param interpOrder
 */
Order1_RFnStrategy::Order1_RFnStrategy( const std::string & splitMethod,
                                        uint32 refineFactor )
    : SplittingStrategy(splitMethod, (2*refineFactor -1) )
{

    int32 icell = -(static_cast<int32>(refineFactor) - 1) ;
    for( uint32 ik=0 ; ik<nbpts_ ; ++ik )
    {
        child_icellx_[ik] = icell ;
        ++icell ;

        child_deltax_[ik] = 0. ;
    }

    int32 inum=1 ;
    for( uint32 ik=0 ; ik<nbpts_ ; ik++ )
    {
        child_weights_[ik] = static_cast<double>(inum)/static_cast<double>(refineFactor) ;

        wtot_ += child_weights_[ik] ;

        if( static_cast<int>(ik-refineFactor+1) < 0 )
            inum++ ;
        else
            inum-- ;
    }

}


