
#include "AMR/particleselector.h"
#include "AMR/mlmdparticleinitializer.h"



void MLMDParticleInitializer::loadParticles( std::vector<Particle>& particlesArray ) const
{

    // the ParticleInitializer has a private access to the ion of the Parent Patch
    for( Particle const & particle: particleSource_.particles() )
    {

        // look if the particle is in the Patch domain
        // the Patch hyperVolume can be represented as a "Box"
        if( selector_(particle) )
        {

            std::vector<Particle> childParticles ;
            // the big particle 'particle' is within the patch
            // so we need to split it and grab its children
            // TODO: add splitting algorithms
//            childParticles[] = split(particle) ;

            // we add each child particles to the array to fill.
            // this array is the particle array of the appropriate species of the new patch
            for( Particle const & child: childParticles)
            {
                particlesArray.push_back(child) ;
            }
        }
    }


}


