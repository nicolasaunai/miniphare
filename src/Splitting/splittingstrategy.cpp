
#include "Splitting/splittingstrategy.h"



SplittingStrategy::SplittingStrategy(std::string name,
                                     uint32 nbpts )
    : splitMethod_{name}, nbpts_{nbpts},
      child_icellx_{ std::vector<int32>(nbpts_, 0) },
      child_deltax_{ std::vector<float>(nbpts_, 0) },
      child_weights_{ std::vector<double>(nbpts_, 0) },
      wtot_{ 0. }
{

}


void SplittingStrategy::split1D(
        const Particle & mother,
        std::vector<Particle> & childParticles ) const
{

    for( uint32 ik=0 ; ik<nbpts_ ; ++ik )
    {
        uint32 icellx = mother.icell[0] + child_icellx_[ik] ;
        float  deltax = mother.delta[0] + child_deltax_[ik] ;

        double weight = mother.weight * child_weights_[ik]/wtot_ ;

        Particle newBorn( weight, mother.charge,
                         {{icellx, mother.icell[1], mother.icell[2]}},
                         {{deltax, mother.delta[1], mother.delta[2]}},
                          mother.v ) ;

        childParticles.push_back( std::move(newBorn) );
    }

}


