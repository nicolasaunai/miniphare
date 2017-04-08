
#include "AMR/particleselector.h"
#include "AMR/mlmdparticleinitializer.h"




void normalizeMotherPosition1D( const GridLayout & refinedLayout,
                               const uint32 refinementRatio,
                               const Particle & mother,
                               Particle & normalizedMother ) ;


void MLMDParticleInitializer::loadParticles( std::vector<Particle>& particlesArray ) const
{
    ParticleSelector const & selector = *selector_ ;

    Particle normalizedMother ;

    // the ParticleInitializer has a private access to the ion of the Parent Patch
    for( Particle const & mother: particleSource_.particles() )
    {
        // look if the 'big' particle is within the Patch domain
        if( selector(mother) )
        {
            normalizeMotherPosition1D( refinedLayout_,
                                     refinementRatio_,
                                     mother, normalizedMother) ;

            std::vector<Particle> childParticles ;
            // We need to split particle and grab its children
            strategy_->split1D( normalizedMother, childParticles ) ;

            // For the considered Species
            // we fill the particle array of the new patch
            for( Particle const & child: childParticles)
            {
                particlesArray.push_back(child) ;
            }
        }
    }


}


void normalizeMotherPosition1D( const GridLayout & refinedLayout,
                              const uint32 refinementRatio,
                              const Particle & mother,
                              Particle & normalizedMother )
{
    normalizedMother = mother ;

    double origX = refinedLayout.origin().x_/refinedLayout.dx() ;

    uint32 icellRefinedOrigX = static_cast<uint32>( std::floor(origX) ) ;

    uint32 icellMother_x = mother.icell[0] * refinementRatio
                         - icellRefinedOrigX ;

    float deltaMother_x = mother.delta[0] * refinementRatio ;

    normalizedMother.icell[0] = icellMother_x ;
    normalizedMother.delta[0] = deltaMother_x ;

}



