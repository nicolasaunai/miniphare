
#include "amr/MLMD/mlmdparticleinitializer.h"
#include "utilities/particleselector.h"
#include "utilities/particleutilities.h"



void MLMDParticleInitializer::loadParticles(std::vector<Particle>& particlesArray) const
{
    ParticleSelector& motherSelector = *selector_;
    IsInBoxSelector childSelector{refinedLayout_.getBox()};

    // the ParticleInitializer has a private access to the ion of the Parent Patch
    for (Particle const& mother : particleSource_.particles())
    {
        // look if the 'big' particle is out but near the PRA domain
        if (motherSelector.pick(mother, coarseLayout_))
        {
            Particle normalizedMother;

            SplittingStrategy::normalizeMotherPosition(coarseLayout_, refinedLayout_, mother,
                                                       normalizedMother);

            std::vector<Particle> childParticles;
            // We need to split particle and grab its children
            strategy_->split1D(normalizedMother, childParticles);

            // For the considered Species
            // we fill the particle array of the new patch
            for (auto& child : childParticles)
            {
                // if (isInRefinedBox(refinedLayout_, child))
                if (childSelector.pick(child, refinedLayout_))
                {
                    particlesArray.push_back(child);
                }
            }
        } // end if mother is selected
    }     // end loop on particles
}
