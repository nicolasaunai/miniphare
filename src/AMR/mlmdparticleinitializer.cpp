
#include "AMR/particleselector.h"
#include "AMR/mlmdparticleinitializer.h"



void MLMDParticleInitializer::loadParticles( std::vector<Particle>& particlesArray ) const
{

    // the ParticleInitializer has a private access to the ion of the Parent Patch
    for( Particle const & particle: particleSource_.particles() )
    {
        // look if the 'big' particle is within the Patch domain
        if( selector_(particle) )
        {
            std::vector<Particle> childParticles ;
            // We need to split particle and grab its children
            strategy_.split1D(particle, childParticles) ;

            // For the considered Species
            // we fill the particle array of the new patch
            for( Particle const & child: childParticles)
            {
                particlesArray.push_back(child) ;
            }
        }
    }


}


