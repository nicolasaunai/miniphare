
#include "AMR/MLMD/mlmdparticleinitializer.h"
#include "AMR/particleselector.h"
#include "AMR/particleutilities.h"

#include "Plasmas/virtualparticle.h"


Particle convertToParticle(VirtualParticle const& part);


void MLMDParticleInitializer::loadParticles(std::vector<Particle>& particlesArray) const
{
    ParticleSelector& selector = *selector_;

    // the ParticleInitializer has a private access to the ion of the Parent Patch
    for (Particle const& mother : particleSource_.particles())
    {
        // look if the 'big' particle is out but near the PRA domain
        if (selector.pick(mother))
        {
            VirtualParticle normalizedMother;

            SplittingStrategy::normalizeMotherPosition(coarseLayout_, refinedLayout_, mother,
                                                       normalizedMother);

            std::vector<VirtualParticle> childParticles;
            // We need to split particle and grab its children
            strategy_->split1D(normalizedMother, childParticles);

            // For the considered Species
            // we fill the particle array of the new patch
            for (VirtualParticle const& child : childParticles)
            {
                if (isInRefinedBox(refinedLayout_, child))
                {
                    particlesArray.push_back(convertToParticle(child));
                }
            }
        }
    }
}



Particle convertToParticle(VirtualParticle const& part)
{
    return Particle{part.weight,
                    part.charge,
                    {{static_cast<uint32>(part.icell[0]), static_cast<uint32>(part.icell[1]),
                      static_cast<uint32>(part.icell[2])}},
                    part.delta,
                    part.v};
}
