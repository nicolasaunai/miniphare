

#include "Splitting/ordern_rf3strategy.h"

#include "Distributions/distribgenerator.h"
#include "Distributions/distribstrategy.h"
#include "Distributions/uniformstrategy.h"

#include "multinomial.h"


OrderN_RF3Strategy::OrderN_RF3Strategy( const std::string & splitMethod,
                                        uint32 refineFactor,
                                        uint32 interpOrder )
    : SplittingStrategy(splitMethod),
      nbpts_{ (2*refineFactor -1) + (refineFactor -1)*(interpOrder-1) },
      child_icellx_{ std::vector<int32>(nbpts_, 0) },
      child_weights_{ std::vector<double>(nbpts_, 0) },
      wtot_{ 0. }
{

    int32 icell = static_cast<int32>( -(nbpts_ - 1)/2. ) ;
    for( uint32 ik=0 ; ik<nbpts_ ; ++ik )
    {
        child_icellx_[ik] = icell ;
        ++icell ;
    }

    uint32 itab = 0 ;
    for( int ik=-(static_cast<int>(interpOrder)+1) ; ik< static_cast<int>(interpOrder)+2 ; ik++ )
    {
        child_weights_[itab] = static_cast<double>( trinomial(interpOrder+1, ik) )/
                static_cast<double>( ipow(refineFactor, interpOrder) ) ;

        wtot_ += child_weights_[itab] ;

        itab++ ;
    }

}




void OrderN_RF3Strategy::split1D(
        const Particle & mother,
        std::vector<Particle> & childParticles ) const
{

    for( uint32 ik=0 ; ik<nbpts_ ; ++ik )
    {
        uint32 icellx = mother.icell[0] + child_icellx_[ik] ;

        double weight = mother.weight * child_weights_[ik]/wtot_ ;

        Particle newBorn( weight, mother.charge,
                         {{icellx, mother.icell[1], mother.icell[2]}},
                          mother.delta, mother.v ) ;

        childParticles.push_back( std::move(newBorn) );
    }

}


